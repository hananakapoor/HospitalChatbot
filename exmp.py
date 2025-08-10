import os
import subprocess

def convert_files_to_wav(input_folder, output_folder):
    # Ensure the output folder exists
    os.makedirs(output_folder, exist_ok=True)

    # Iterate over all files in the input directory
    for filename in os.listdir(input_folder):
        file_path = os.path.join(input_folder, filename)

        # Check if the file is not a directory and if it is a non-WAV file
        if os.path.isfile(file_path) and not filename.endswith(".wav"):
            # Construct the output file path (with the same name but with a .wav extension in the output folder)
            output_file_path = os.path.join(output_folder, filename.rsplit('.', 1)[0] + ".wav")

            # Construct the ffmpeg command to convert to WAV with specified parameters
            command = [
                "ffmpeg",
                "-i", file_path,  # Input file
                "-ar", "44100",    # Sample rate
                "-ac", "2",        # Number of audio channels (stereo)
                "-f", "wav",       # Output format
                output_file_path   # Output file path
            ]

            try:
                # Execute the ffmpeg command
                subprocess.run(command, check=True)
                print(f"Converted: {filename} to WAV format")

            except subprocess.CalledProcessError as e:
                print(f"Error converting {filename}: {e}")

# Example usage:
input_folder = "voice2"  # Original folder containing MP3 or other files
output_folder = "voice3"    # Folder where converted WAV files will be saved

convert_files_to_wav(input_folder, output_folder)
