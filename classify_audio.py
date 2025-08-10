import requests
import os
import sys

def send_audio_for_prediction(api_url, audio_file_path):
    """Send audio file to the Flask API for prediction"""
    try:
        with open(audio_file_path, 'rb') as f:
            files = {'file': (os.path.basename(audio_file_path), f, 'audio/wav')}
            response = requests.post(api_url, files=files)

        if response.status_code == 200:
            return response.json().get('predicted_class', 'Unknown')
        else:
            print(f"Error: {response.status_code} - {response.text}", file=sys.stderr)
            return 'Unknown'
    except Exception as e:
        print(f"Error sending file: {e}", file=sys.stderr)
        return 'Unknown'

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 classify_audio.py <input_audio>")
        sys.exit(1)

    # Default API URL (change this to your server's address)
    API_URL = "http://localhost:5000/transcribe"

    audio_path = sys.argv[1]

    if not os.path.isfile(audio_path):
        print(f"Error: {audio_path} is not a file", file=sys.stderr)
        sys.exit(1)

    predicted_class = send_audio_for_prediction(API_URL, audio_path)
    print(predicted_class)  # Output just the predicted class or "Unknown"
