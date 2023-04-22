import requests
import os


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


def download_from_bird_json_infos(recordings_folder, bird_json):
    len_bird_recordings = len(bird_json)
    for index, recording in enumerate(bird_json):
        print("Downloading file:", index + 1, "out of", len_bird_recordings, "(", recording["file-name"], ")")
        download_file(recording["file"], os.path.join(recordings_folder, recording["gen"] + "_" + recording["sp"]),
                      recording["q"] + "_" + recording["file-name"])