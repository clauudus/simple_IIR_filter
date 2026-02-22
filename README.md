# Project: simple_lp (one-pole low-pass demo)

## Files:
  - simple_lp.cpp               : C++ program that applies a one-pole low-pass filter
  - run_and_plot.py             : Python script that compiles/executes, calcuates RMS/centroid and generates before_after.png
  - demo.sh                     : Compiles and runs everything
  - Documentation_Results.pdf   : Explanation of two results from running the scripts (5000 Hz and 1000 Hz)

## Usage:
### Installing Dependencies:
```
sudo apt install libsndfile1-dev
pip3 install numpy librosa matplotlib soundfile
```

### Make Executable:
```
chmod +x demo.sh
```

### Execute:
```
./demo.sh input.wav output.wav 5000
```

### Results:
  - output.wav (processed)
  - before_after.png
  - In terminal: RMS and spectral centroid before/after

### Extra Notes:
In this repo I didn't include the WAV file. In my case it's because I don't have the WAV file license. <br />
To run the tests on this repo I used Isengards song "In The Halls And Chambers Of Stardust The Crystallic" purely because I really like the song. (to be more spacific, 30 seconds of the song). <br />
This is a very very (and I cannot stress this enough) very simple project, for myself, only to understand a bit better the audio processing world. <br />
Since I am a very begginer, please, feel more than free to give any comments and do any improvement on the code :)
