# imports

import os
from download_dataset import get_bird_json, download_from_bird_json_infos
from prepare_and_clean_dataset import split_audio_file

# parameters
main_bird = "Parus major"
main_bird_quality = ("A", "B", "C")
number_of_main_bird_recordings = 5

test_bird_1 = "Turdus merula"
test_bird_1_quality = ("A", "B", "C")
number_of_test_bird_1_recordings = 1

test_bird_2 = "Fringilla coelebs"
test_bird_2_quality = ("A", "B", "C")
number_of_test_bird_2_recordings = 1

recordings_folder = "recordings"

# dl data
main_bird_recordings_json = get_bird_json(main_bird, main_bird_quality, number_of_main_bird_recordings)
download_from_bird_json_infos(recordings_folder, main_bird_recordings_json)

test_bird_1_recordings_json = get_bird_json(test_bird_1, test_bird_1_quality, number_of_test_bird_1_recordings)
download_from_bird_json_infos(recordings_folder, test_bird_1_recordings_json)

test_bird_2_recordings_json = get_bird_json(test_bird_2, test_bird_2_quality, number_of_test_bird_2_recordings)
download_from_bird_json_infos(recordings_folder, test_bird_2_recordings_json)

# clean data

for bird_type in os.listdir(recordings_folder):
    bird_folder = os.path.join(recordings_folder, bird_type)
    for recording in os.listdir(bird_folder):
        recording_path = os.path.join(bird_folder, recording)
        split_audio_file(recording_path, bird_folder, recording.split(".")[0])
