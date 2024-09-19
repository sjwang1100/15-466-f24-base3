import numpy as np
import matplotlib.pyplot as plt
from pydub import AudioSegment
import os

def plot_waveform(audio_file):
    # Load the audio file
    audio = AudioSegment.from_file(audio_file)
    
    # Convert audio to mono and get raw data
    audio = audio.set_channels(1)  # Ensure it's mono
    samples = np.array(audio.get_array_of_samples())
    
    # Get frame rate and duration
    sample_rate = audio.frame_rate
    duration = len(samples) / sample_rate

    # Time axis
    time = np.linspace(0, duration, num=len(samples))

    # Extract file name without extension
    file_name = os.path.splitext(os.path.basename(audio_file))[0]

    # Generate output file names
    normal_output = f"{file_name}-1.png"
    flipped_output = f"{file_name}-2.png"

    # Function to create and save the plot
    def create_plot(time, samples, output_file, flip=False):
        plt.figure(figsize=(12, 6))
        if flip:
            samples = -samples  # Invert the waveform

        # Plot the waveform
        plt.plot(time, samples, color='cyan', alpha=0.8)

        # Customize the plot (style)
        plt.title('Stylish Audio Waveform', fontsize=18, color='white')
        plt.xlabel('Time (s)', fontsize=14, color='white')
        plt.ylabel('Amplitude', fontsize=14, color='white')
        plt.grid(True, alpha=0.3, color='gray')

        # Set background to black
        plt.gca().set_facecolor('black')
        plt.gcf().set_facecolor('black')

        # Remove axis lines and ticks
        plt.gca().spines['top'].set_color('none')
        plt.gca().spines['right'].set_color('none')
        plt.gca().spines['left'].set_color('white')
        plt.gca().spines['bottom'].set_color('white')
        plt.gca().tick_params(colors='white')

        # Save the figure
        plt.savefig(output_file, bbox_inches='tight', transparent=True)
        plt.close()

    # Create and save the normal waveform
    create_plot(time, samples, normal_output)

    # Create and save the flipped waveform
    create_plot(time, samples, flipped_output, flip=True)

    print(f"Waveforms saved as {normal_output} and {flipped_output}")

# Example usage:
audio_file = 'D:/OneDrive - andrew.cmu.edu/FA 24/15666 Game Design/Game3/15-466-f24-base3/dist/Random 6.opus'  # Replace with your audio file path
plot_waveform(audio_file)
