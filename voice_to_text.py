from flask import Flask, request, jsonify, render_template_string
import torch
import os
import tempfile
import json
from pydub import AudioSegment
from difflib import get_close_matches, SequenceMatcher
import logging
import time


# Set up minimal logging - only critical errors
logging.basicConfig(level=logging.ERROR, 
                    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

app = Flask(__name__)

# List of allowed phrases
ALLOWED_PHRASES = [
    "no", "book a doctor","book", "doctor", "call", "ambulance","call an ambulance", "admission","diagnostic services", "diagnostic", "diagnose", "bill", "pay", "payment", "insurance", "pharmacy", "medicine", "paediatrics", "cardiology", "heart", "ent", "general", "general medicine", "neurology", "dermatology", "oncology", "ok", "ip", "op", "i p", "o p", "xray", "c t scan", "c t", "m r i", "m r i scan", "blood", "bloodtest", "yes"
]

def process_audio(audio_file_path):
    """
    Convert audio to proper format for the Silero STT model.
    """
    try:
        audio = AudioSegment.from_file(audio_file_path)
        audio = audio.set_frame_rate(16000).set_channels(1).set_sample_width(2)
        
        with tempfile.NamedTemporaryFile(suffix=".wav", delete=False) as temp_file:
            fixed_audio = temp_file.name
            audio.export(fixed_audio, format="wav")
            return fixed_audio
    except Exception as e:
        raise

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
model, decoder, utils = torch.hub.load(
    repo_or_dir='snakers4/silero-models',
    model='silero_stt',
    language='en',
    device=device
)
(read_batch, split_into_batches, read_audio, prepare_model_input) = utils

def transcribe_audio(file_path):
    """
    Transcribe audio using Silero STT model and match to allowed phrases.
    Prints execution time every second.
    Returns predicted class (matched phrase) with confidence scores.
    """
    try:
        # Read and process the audio file
        test_files = [file_path]
        batches = split_into_batches(test_files, batch_size=1)
        input_data = prepare_model_input(read_batch(batches[0]), device=device)
        
        start_time = time.time()
        # Perform speech-to-text
        output = model(input_data)
        
        # Timer update
        print(f"Core Execution time: {round(time.time() - start_time, 2)} seconds")
        
        # Get raw transcription
        transcriptions = [decoder(example.cpu()) for example in output]
        raw_transcription = transcriptions[0].lower().strip() if transcriptions else ""
        print(f"'raw_transcription': '{raw_transcription}'")
        
        if not raw_transcription:
            print("'matched_phrase': 'No matching phrase found'")
            return {
                'raw_transcription': "(No speech detected)",
                'predicted_class': "No matching phrase found",
                'confidence': 0.0,
                'all_predictions': {phrase: 0.0 for phrase in ALLOWED_PHRASES}
            }, None
        
        # Calculate confidence scores for all allowed phrases
        all_predictions = {phrase: SequenceMatcher(None, raw_transcription, phrase).ratio() for phrase in ALLOWED_PHRASES}
        
        # Find the best match
        predicted_class = max(all_predictions, key=all_predictions.get)
        confidence = all_predictions[predicted_class]
        
        # Only accept matches with confidence above threshold
        if confidence < 0.5:
            predicted_class = "No matching phrase found"
        
        print(f"'matched_phrase': '{predicted_class}'")
        
        # Final execution time
        print(f"Total execution time: {round(time.time() - start_time, 2)} seconds")
        
        return {
            'raw_transcription': raw_transcription,
            'predicted_class': predicted_class,
            'confidence': round(confidence, 4),
            'all_predictions': {k: round(v, 4) for k, v in all_predictions.items()}
        }, None
        
    except Exception as e:
        return None, str(e)


@app.route('/transcribe', methods=['POST'])
def speech_processing():
    """
    API endpoint to convert speech in an audio file to text and match to allowed phrases.
    """
    temp_path = None
    fixed_audio = None
    
    try:
        # Check if audio file was uploaded
        if 'file' not in request.files:
            return jsonify({"error": "No file provided"}), 400
        
        file = request.files['file']
        if file.filename == '':
            return jsonify({"error": "No file selected"}), 400
        
        # Save uploaded file temporarily
        with tempfile.NamedTemporaryFile(delete=False, suffix=os.path.splitext(file.filename)[1]) as temp:
            temp_path = temp.name
            file.save(temp_path)
        
        # Process audio to correct format
        fixed_audio = process_audio(temp_path)
        
        # Transcribe audio and match to phrases
        result, error = transcribe_audio(fixed_audio)
        
        if error:
            return jsonify({"error": f"Processing error: {error}"}), 500
        
        # Clean up temporary files
        if temp_path and os.path.exists(temp_path):
            os.unlink(temp_path)
        
        if fixed_audio and os.path.exists(fixed_audio):
            os.unlink(fixed_audio)
        
        return jsonify(result)
        
    except Exception as e:
        # Clean up temporary files if they exist
        if temp_path and os.path.exists(temp_path):
            try:
                os.unlink(temp_path)
            except:
                pass
                
        if fixed_audio and os.path.exists(fixed_audio):
            try:
                os.unlink(fixed_audio)
            except:
                pass
        
        # Return a friendly error message
        return jsonify({
            "error": f"Processing error: {str(e)}",
            "details": "See server logs for more information"
        }), 500


@app.route('/', methods=['GET'])
def index():
    return render_template_string(HTML_TEMPLATE, allowed_phrases=ALLOWED_PHRASES)

if __name__ == "__main__":
    # Disable Flask's default logging
    logging.getLogger('werkzeug').setLevel(logging.ERROR)
    app.logger.disabled = True
    
    # Run the app without debug output
    app.run(debug=False, host='0.0.0.0', port=5000)
