import speech_recognition as sr
from pydub import AudioSegment
import sys

def speech_processing(input_audio):
    """
    Convert speech in an audio file to text.
    """
    try:
        # Convert input audio to proper WAV format
        audio = AudioSegment.from_file(input_audio)
        audio = audio.set_frame_rate(16000).set_channels(1).set_sample_width(2)
        fixed_audio = "audio_fixed.wav"
        audio.export(fixed_audio, format="wav")

        # Speech-to-Text
        recognizer = sr.Recognizer()
        with sr.AudioFile(fixed_audio) as source:
            audio_data = recognizer.record(source)
            text = recognizer.recognize_google(audio_data)
            print(text)  # Output text so C++ can capture it
            return text

    except sr.UnknownValueError:
        print("Error: Could not understand the audio")
    except sr.RequestError as e:
        print(f"Error: Speech Recognition request failed; {e}")
    except Exception as e:
        print(f"Error: {e}")

# Run the function
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 stt.py <input_audio>")
    else:
        speech_processing(sys.argv[1])
