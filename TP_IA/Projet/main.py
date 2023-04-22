import requests
import os

main_bird = "Parus major"
main_bird_quality = ("A", "B", "C")

test_bird_1 = "Turdus merula"
test_bird_1_quality = ("A", "B", "C")

test_bird_2 = "Fringilla coelebs"
test_bird_2_quality = ("A", "B", "C")

recordings_folder = "recordings"


def download_file(link, directory, filename):
    if os.path.exists(os.path.join(directory, filename)):
        print("File exists, skipping")
        return

    if not os.path.exists(directory):
        path = directory.split(os.sep)
        for i in range(1, len(path) + 1):
            sub_dir = os.path.join(*path[:i])
            if not os.path.exists(sub_dir):
                os.makedirs(sub_dir)

    with open(os.path.join(directory, filename), 'wb') as f:
        f.write(requests.get(link).content)
    f.close()


def get_bird_json(bird_name, recording_quality, number_of_recordings):
    api_base_link = "https://xeno-canto.org/api/2/recordings?query="
    quality_header = "q:"
    bird_name = "+".join(bird_name.split(" "))

    final_json = []

    for quality in recording_quality:
        print("Getting recordings data for", bird_name, "with quality", quality)
        api_link = api_base_link + bird_name + "+" + quality_header + quality
        json_recording = requests.get(api_link).json()["recordings"]
        final_json.extend(json_recording)

    final_json = [recording for recording in final_json if "song" in recording["type"]]
    return final_json[:number_of_recordings]


def download_from_bird_json_infos(bird_json):
    len_bird_recordings = len(bird_json)
    for index, recording in enumerate(bird_json):
        print("Downloading file:", index + 1, "out of", len_bird_recordings, "(", recording["file-name"], ")")
        download_file(recording["file"], os.path.join(recordings_folder, recording["gen"] + "_" + recording["sp"]),
                      recording["q"] + "_" + recording["file-name"])


# main

# # download raw sound files

main_bird_recordings_json = get_bird_json(main_bird, main_bird_quality, 5)
download_from_bird_json_infos(main_bird_recordings_json)

test_bird_1_recordings_json = get_bird_json(test_bird_1, test_bird_1_quality, 1)
download_from_bird_json_infos(test_bird_1_recordings_json)

test_bird_2_recordings_json = get_bird_json(test_bird_2, test_bird_2_quality, 1)
download_from_bird_json_infos(test_bird_2_recordings_json)

# # split sound files into 1 second chunks

import os
import librosa
import numpy as np
import soundfile as sf


def split_audio_file(audio_file_path, output_folder, output_file_name, split_length=1):
    audio_signal, sample_rate = librosa.load(audio_file_path, sr=None)

    len_audio_signal = len(audio_signal)

    split_length_samples = split_length * sample_rate
    audio_signal = audio_signal[:len_audio_signal - len_audio_signal % split_length_samples]
    audio_signal = np.split(audio_signal, len(audio_signal) / split_length_samples)

    number_of_files = str(int(len_audio_signal/sample_rate))
    for index, y_split in enumerate(audio_signal):
        sf.write(
            os.path.join(output_folder, output_file_name + "_" + str(index+1) + "_of_" + number_of_files + ".wav"),
            y_split, sample_rate)
    #os.remove(audio_file_path)


for bird_type in os.listdir(recordings_folder):
    bird_folder = os.path.join(recordings_folder, bird_type)
    for recording in os.listdir(bird_folder):
        recording_path = os.path.join(bird_folder, recording)
        split_audio_file(recording_path, bird_folder, recording.split(".")[0])
