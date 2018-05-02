import sys
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import time
import threading
import random
import pandas as pd
import numpy

# index = np.arange(0,1024)
# columns=['input', 'hamming', 'psd']
# data = np.array([np.zeros(1024)] * 3).T
# RAW = pd.DataFrame(data, index=index, columns=columns)

#S = np.arange(1024)

# This just simulates reading from a socket.
def data_listener():
    for line in sys.stdin:
        line = line.strip('\n')
        n = int  (line.split(',')[0])
        s = float(line.split(',')[1])
        w = float(line.split(',')[2])
        p = float(line.split(',')[3])
        # global S
        # S[n] = s
        print line

# def plot(ax, style):
#     return ax.plot(xrange(0,1024), S, style, animated=True)[0]
#
# class MyFuncAnimation(animation.FuncAnimation):
#     """
#     Unfortunately, it seems that the _blit_clear method of the Animation
#     class contains an error in several matplotlib verions
#     That's why, I fork it here and insert the latest git version of
#     the function.
#     """
#     def _blit_clear(self, artists, bg_cache):
#         # Get a list of the axes that need clearing from the artists that
#         # have been drawn. Grab the appropriate saved background from the
#         # cache and restore.
#         axes = set(a.axes for a in artists)
#         for a in axes:
#             if a in bg_cache: # this is the previously missing line
#                 a.figure.canvas.restore_region(bg_cache[a])
#
# def animate(i):
#     for j, line in enumerate(lines, start=1):line.set_ydata(S)
#     return lines
def mel():
    low_freq_mel = 0
    sample_rate = 16000
    nfilt = 26
    NFFT = 1024
    high_freq_mel = (2595.0 * numpy.log10(1.0 + (sample_rate / 2.0) / 700.0))  # Convert Hz to Mel
    mel_points = numpy.linspace(low_freq_mel, high_freq_mel, nfilt + 2)  # Equally spaced in Mel scale
    hz_points = (700.0 * (10.0**(mel_points / 2595.0) - 1.0))  # Convert Mel to Hz
    bin = numpy.floor((NFFT + 1) * hz_points / sample_rate)
    print "mel_points:", (mel_points)
    print "bins: ", bin, len(bin)

    fbank = numpy.zeros((nfilt, int(numpy.floor(NFFT / 2 + 1))))
    #print "fbank:", len(fbank[0])

    for m in range(1, nfilt + 1):
        f_m_minus = int(bin[m - 1])   # left
        f_m = int(bin[m])             # center
        f_m_plus = int(bin[m + 1])    # right
        print "m-1, m, m+1", f_m_minus, f_m, f_m_plus,range(f_m_minus, f_m)

    for k in range(f_m_minus, f_m):
        fbank[m - 1, k] = (k - bin[m - 1]) / (bin[m] - bin[m - 1])

    for k in range(f_m, f_m_plus):
        fbank[m - 1, k] = (bin[m + 1] - k) / (bin[m + 1] - bin[m])

    frames = numpy.hamming(0.064*16000)
    mag_frames = numpy.absolute(numpy.fft.rfft(frames, NFFT))  # Magnitude of the FFT
    pow_frames = ((1.0 / NFFT) * ((mag_frames) ** 2))
    filter_banks = numpy.dot(pow_frames, fbank.T)
    print numpy.shape(pow_frames), numpy.shape(fbank.T), numpy.shape(filter_banks)
if __name__ == '__main__':
    mel()
    # styles = ['r-', 'g-', 'b-']
    # fig, axes = plt.subplots(nrows=3)
    #while True:
    #data_listener()
    # thread = threading.Thread(target=data_listener)
    # thread.daemon = True
    # thread.start()
    # #
    # # lines = [plot(ax, style) for ax, style in zip(axes, styles)]
    # # ani = MyFuncAnimation(fig, animate, xrange(1, 20), interval=0, blit=True)
    # #fig.show()
    # #
    # thread.join()


    #fig, axes = plt.subplots(nrows=3)
    #
    # while True:
    #     print 'hello'

    #
    # # initialize figure
    # fig = plt.figure()
    # ax = fig.add_subplot(111)
    # ax.axis('auto')
    # ln, = ax.plot(xrange(0,513), RAW['psd'][:513])
    # plt.ion()
    # plt.show()
    #
    # while True:
    #     plt.pause(1)
    #     ln.set_ydata(RAW['psd'][:513])
    #     ax.axis([0, 512, min(RAW['psd'][:513]), max(RAW['psd'][:513])])
    #     #plt.plot([])
    #     plt.draw()
#global RAW
#RAW.iloc[n] = [s, w, p]
#print s
#global S
#S[n] = s

#print RAW.iloc[n]
#print n, s, w, p
#RAW.set_value(n, 'input', s)
#RAW.set_value(n, 'hamming', w)

#RAW.set_value(n, {'input' : s, 'psd' : p, 'hamming' : w})
#RAW[n] =
#RAW = RAW.append({n, s, w, p})
#RAW[n] = {'input' :  s, 'hamming' : w, 'psd' : p}
#print RAW
