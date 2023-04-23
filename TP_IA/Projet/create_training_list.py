import os
import numpy as np

# create train, test and text pointers
recordings_folder = "recordings"
main_bird = "Parus major"

file_name = "testing_list.txt"

count = 0
with open(os.path.join(recordings_folder, file_name), "w", encoding="utf-8") as f:
    for bird_type in os.listdir(recordings_folder):
        bird_folder = os.path.join(recordings_folder, bird_type)
        if os.path.isdir(bird_folder):
            for recording in os.listdir(bird_folder):
                sound_filename = os.path.join(bird_folder, recording)
                if os.path.isfile(sound_filename) and "splitted_" in sound_filename:
                    if np.random.rand() > 0.3:
                        f.write(sound_filename + "\n")
                        count += 1
    ratio = count / len(os.listdir(os.path.join(recordings_folder, main_bird.replace(" ", "_"))))
    number_of_main_bird_recordings = count / (
                sum(os.path.isdir(os.path.join(recordings_folder, f)) for f in os.listdir(recordings_folder)) - 1)

    count = 0

    for recording in os.listdir(os.path.join(recordings_folder, main_bird.replace(" ", "_"))):
        if count > number_of_main_bird_recordings:
            break

        # if random number between 0 and 1 is greater than 0.3 then add to testing_list.txt
        if np.random.rand() > ratio:
            count += 1
            f.write(os.path.join(recordings_folder, main_bird.replace(" ", "_"), recording) + "\n")

print("Done")
