import requests
import os

main_bird = "Parus major"
main_bird_quality = "A"

test_bird_1 = "Turdus merula"
test_bird_1_quality = "A"

test_bird_2 = "Fringilla-coelebs"
test_bird_2_quality = "A"

output_folder = "recordings"


def download_file(link, directory, filename):
    if os.path.exists(os.path.join(directory, filename)):
        print("File exists, skipping")
        return

    if not os.path.exists(directory):
        os.mkdir(directory)

    with open(os.path.join(directory, filename), 'wb') as f:
        f.write(requests.get(link).content)
    f.close()


def get_bird_json(bird_name, recording_quality, number_of_recordings):
    if recording_quality not in ["A", "B", "C", "D", "E"]:
        print("Wrong quality")
        return

    api_base_link = "https://xeno-canto.org/api/2/recordings?query="
    quality_header = "q:"
    bird_name = "+".join(bird_name.split(" "))
    api_link = api_base_link + bird_name + "+" + quality_header + recording_quality
    response = requests.get(api_link)

    return [json_recording for json_recording in response.json()["recordings"] if "song" in json_recording["type"]][
           :number_of_recordings]


def download_from_bird_json_infos(bird_json):
    len_bird_recordings = len(bird_json)
    for index, recording in enumerate(bird_json):
        print("Downloading file:", index + 1, "out of", len_bird_recordings, "(", recording["file-name"], ")")
        download_file(recording["file"], os.path.join(output_folder, recording["gen"]+"_"+recording["sp"]), recording["file-name"])


# main

main_bird_recordings_json = get_bird_json(main_bird, main_bird_quality, 100)
download_from_bird_json_infos(main_bird_recordings_json)

test_bird_1_recordings_json = get_bird_json(test_bird_1, test_bird_1_quality, 100)
download_from_bird_json_infos(test_bird_1_recordings_json)

test_bird_2_recordings_json = get_bird_json(test_bird_2, test_bird_2_quality, 100)
download_from_bird_json_infos(test_bird_2_recordings_json)

