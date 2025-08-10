import os
from pydub import AudioSegment

# Define the input and output folder paths
input_folder = 'voice3'  # Change this to the folder with your original audio files
output_folder = 'voice4'    # The folder to save the converted files

# Make sure the output folder exists
os.makedirs(output_folder, exist_ok=True)

# Loop through all files in the input folder
for filename in os.listdir(input_folder):
    if filename.endswith(".wav"):
        input_file = os.path.join(input_folder, filename)

        try:
            # Load the audio file
            audio = AudioSegment.from_wav(input_file)

            # Change sample rate to 16000 Hz and convert to mono (1 channel)
            audio = audio.set_frame_rate(16000).set_channels(1)

            # Save the modified file to the output folder
            output_file = os.path.join(output_folder, filename)
            audio.export(output_file, format="wav")
            print(f"Processed {filename} -> saved to {output_file}")

        except Exception as e:
            print(f"Error processing {filename}: {e}")
