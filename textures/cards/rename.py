import os

# Rank and suit mappings
rank_map = {
    "ace": 0, "2": 1, "3": 2, "4": 3, "5": 4, "6": 5,
    "7": 6, "8": 7, "9": 8, "10": 9,
    "jack": 10, "queen": 11, "king": 12
}

suit_map = {
    "spades": 0,
    "hearts": 13,
    "diamonds": 26,
    "clubs": 39
}

# Rename files in the current directory
for filename in os.listdir('.'):
    if filename.endswith('.png'):
        name_part = filename[:-4]  # Remove .png
        try:
            rank_str, _, suit_str = name_part.partition('_of_')
            rank = rank_map[rank_str.lower()]
            suit_offset = suit_map[suit_str.lower()]
            card_number = suit_offset + rank
            new_name = f"card{card_number}.png"
            print(f"Renaming '{filename}' to '{new_name}'")
            os.rename(filename, new_name)
        except KeyError:
            print(f"Skipping unrecognized file format: {filename}")