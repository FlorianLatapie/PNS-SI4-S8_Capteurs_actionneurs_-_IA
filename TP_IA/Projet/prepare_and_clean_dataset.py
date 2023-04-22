import os
import librosa
import numpy as np
import soundfile as sf


def split_audio_file(audio_file_path, output_folder, output_file_name, split_length=1):
    # if filename starts with splitted_ then skip
    if output_file_name.startswith("splitted_"):
        print("file already splitted, skipping")
        return
    if any("splitted_"+output_file_name in f for f in os.listdir(output_folder)):
        print("file already exists, skipping")
        return
    audio_signal, sample_rate = librosa.load(audio_file_path, sr=None)

    len_audio_signal = len(audio_signal)

    split_length_samples = split_length * sample_rate
    audio_signal = audio_signal[:len_audio_signal - len_audio_signal % split_length_samples]
    if len(audio_signal) > split_length:
        audio_signal = np.split(audio_signal, len(audio_signal) / split_length_samples)

    number_of_files = str(int(len_audio_signal / sample_rate))
    for index, y_split in enumerate(audio_signal):
        sf.write(
            os.path.join(output_folder,
                         "splitted_" + output_file_name + "_" + str(index + 1) + "_of_" + number_of_files + ".wav"),
            y_split, sample_rate)
    # os.remove(audio_file_path)
