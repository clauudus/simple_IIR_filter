# run_and_plot.py
# Usage: python3 run_and_plot.py input.wav output.wav [cutoff_hz]
import sys, subprocess
import numpy as np
import librosa
import librosa.display
import matplotlib.pyplot as plt

if len(sys.argv) < 3:
    print("Usage: python3 run_and_plot.py input.wav output.wav [cutoff_hz]")
    sys.exit(1)
inp = sys.argv[1]; out = sys.argv[2]
fc = sys.argv[3] if len(sys.argv)>3 else None

# Compile binary if missing
import os
if not os.path.exists('./simple_lp'):
    print("Compiling simple_lp...")
    subprocess.check_call(['g++','-O3','-std=c++17','simple_lp.cpp','-lsndfile','-o','simple_lp'])

# Run processor
cmd = ['./simple_lp', inp, out] + ([fc] if fc else [])
print("Running:", ' '.join(cmd))
subprocess.check_call(cmd)

# Load audio (mono mix for plotting)
y1, sr = librosa.load(inp, sr=None, mono=True)
y2, sr2 = librosa.load(out, sr=None, mono=True)
assert sr==sr2

# Metrics
def metrics(y, sr):
    rms = librosa.feature.rms(y=y)[0]
    cent = librosa.feature.spectral_centroid(y=y, sr=sr)[0]
    return np.mean(rms), np.mean(cent)
r1, c1 = metrics(y1, sr)
r2, c2 = metrics(y2, sr)
print(f"RMS mean: before={r1:.6f}, after={r2:.6f}")
print(f"Spectral centroid (Hz): before={c1:.1f}, after={c2:.1f}")

# Plot waveform + spectrogram before/after
plt.figure(figsize=(10,8))

plt.subplot(2,2,1)
librosa.display.waveshow(y1, sr=sr)
plt.title('Waveform - original')

plt.subplot(2,2,2)
librosa.display.waveshow(y2, sr=sr)
plt.title('Waveform - processed')

plt.subplot(2,2,3)
D1 = librosa.amplitude_to_db(np.abs(librosa.stft(y1, n_fft=2048)), ref=np.max)
librosa.display.specshow(D1, sr=sr, x_axis='time', y_axis='log')
plt.title('Spectrogram - original')
plt.colorbar(format='%+2.0f dB')

plt.subplot(2,2,4)
D2 = librosa.amplitude_to_db(np.abs(librosa.stft(y2, n_fft=2048)), ref=np.max)
librosa.display.specshow(D2, sr=sr, x_axis='time', y_axis='log')
plt.title('Spectrogram - processed')
plt.colorbar(format='%+2.0f dB')

plt.tight_layout()
plt.savefig('before_after.png', dpi=150)
print("Saved figure: before_after.png")
