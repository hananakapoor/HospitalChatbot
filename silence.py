from pydub import AudioSegment
from pydub.silence import split_on_silence
import os

def remove_silence(input_folder, output_folder, silence_thresh=-40, min_silence_len=1000):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    # Loop through all MP3 files in the input folder
    for file_name in os.listdir(input_folder):
        if file_name.endswith(".mp3"):
            # Load the audio file
            file_path = os.path.join(input_folder, file_name)
            audio = AudioSegment.from_mp3(file_path)

            # Split the audio into chunks based on silence
            chunks = split_on_silence(audio,
                                      min_silence_len=min_silence_len,
                                      silence_thresh=silence_thresh)

            # Concatenate the non-silent chunks
            non_silent_audio = AudioSegment.empty()
            for chunk in chunks:
                non_silent_audio += chunk

            # Save the processed audio to the output folder with the same file name
            output_path = os.path.join(output_folder, file_name)
            non_silent_audio.export(output_path, format="mp3")
            print(f"Processed {file_name} and saved to {output_path}")

# Define the input folder and output folder
input_folder = 'voice1'
output_folder = 'voice2'

# Call the function to remove silence
remove_silence(input_folder, output_folder)
