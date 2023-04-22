import os

# create train, test and text pointers
recordings_folder = "recordings"
main_bird = "Parus major"

file_name = "testing_list.txt"

# saved to recordings folder
# put all filnames of not main bird recordings in testing_list.txt
# put half of the total number of current in testing_list.txt of main bird recordings

count = 0
with open(os.path.join(recordings_folder, file_name), "w", encoding="utf-8") as f:
    for bird_type in os.listdir(recordings_folder):
        if bird_type != main_bird.replace(" ", "_"):
            bird_folder = os.path.join(recordings_folder, bird_type)
            if os.path.isdir(bird_folder):
                for recording in os.listdir(bird_folder):
                    sound_filename = os.path.join(bird_folder, recording)
                    if os.path.isfile(sound_filename) and "_of_" in sound_filename:
                        f.write(sound_filename + "\n")
                        count += 1
    number_of_main_bird_recordings = count / 2
    for index, recording in enumerate(os.listdir(os.path.join(recordings_folder, main_bird.replace(" ", "_")))):
        if index > number_of_main_bird_recordings:
            break
        f.write(os.path.join(recordings_folder, main_bird.replace(" ", "_"), recording) + "\n")


print("Done")
