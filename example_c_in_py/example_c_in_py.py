import asyncio
import websockets
import ctypes
from datum import *
import numpy as np
import copy
from ContextData import *
from mapping import *
import time

sample_index_offset_zs = 0

datum_decoder = ctypes.cdll.LoadLibrary('./datum_decoder.so')

def free_memory(file):
    file.free_all()

def read_waveforms_f(filename: str,
                     CardIndex: [int] = None,
                     ChipIndex: [] = None,
                     ChannelIndex: [] = None,
                     filter_len=0):
    with open(filename, 'rb') as f:
        data = f.read()
    return read_waveforms_binary(data, CardIndex, ChipIndex, ChannelIndex, filter_len)

def read_waveforms_binary(data: bytearray,
                     CardIndex: [int] = None,
                     ChipIndex: [] = None,
                     ChannelIndex: [] = None,
                     filter_len=0):
    # peak_thr = 100
    # datum_decoder = ctypes.CDLL(dll_file)
    w_len = ctypes.c_int(0)
    waveforms = ctypes.pointer(DatumContextWaveformC())

    # ushort_array = ctypes.c_ushort * len(data)
    data_len = len(data)//2

    datum_decoder.get_waveform_from_binary_single(bytes(data),
                                        ctypes.c_int(data_len),
                                        ctypes.c_uint(ItemTypes.IT_START_OF_EVENT.value),
                                        ctypes.byref(waveforms),
                                        ctypes.byref(w_len));


    # datum_decoder.get_waveform_from_file_single(ctypes.c_char_p(filename.encode('utf-8')),
    #                                      ctypes.c_int(peak_thr),
    #                                      ctypes.c_uint(ItemTypes.IT_START_OF_EVENT.value),
    #                                      ctypes.byref(waveforms),
    #                                      ctypes.byref(w_len))

    waveforms_list = []

    # print("Reading binary data waveforms total {0:d}, finished in {1:.2f}.".format(
    #     w_len.value, (end - start)))
    for idx in range(int(w_len.value)):
        waveforms_list.append(copy.copy(waveforms[idx]))

    free_memory(datum_decoder)
    return waveforms_list

async def loop():
    uri = "ws://128.141.115.58:8001"
    counter = 0
    async with websockets.connect(uri, max_size=2**30, ping_interval=None) as websocket:
        print('Starting the communication')
        while True:
            answer = await websocket.recv()
            print(f"Received {len(answer)} bytes")
            display_data(answer)
            # with open(f"out{counter}.aqs", 'wb') as f:
            #     f.write(answer)
            counter += 1

def display_data(data):
    wfs = read_waveforms_binary(data)
    print(f"Got {len(wfs)} waveforms")
    cards = np.array([wf.CardIndex for wf in wfs])
    chips = np.array([wf.ChipIndex for wf in wfs])
    channels = np.array([wf.ChannelIndex for wf in wfs])
    samples = np.array([wf.AdcSample for wf in wfs])

    Qs = {}
    Ts = {}
    for i in range(8):
        Qs[i] = np.zeros((nPadx, nPady))
        Ts[i] = np.zeros((nPadx, nPady))

    for card, chip, channel, sample in zip(cards, chips, channels, samples):
        if channel not in [15, 28, 53, 66] and channel > 2 and channel < 79:
            if card > 7:
                continue
            x = mapping.i(chip//n_chips, chip%n_chips, daq.connector(channel))
            y = mapping.j(chip//n_chips, chip%n_chips, daq.connector(channel))
            Q = np.max(sample)
            T = np.argmax(sample)

            Qs[card][x][y] = Q
            Ts[card][x][y] = T

    row_1 = np.concatenate((Qs[0],
                                np.ones((2, nPady))*(-1),
                                Qs[1],
                                np.ones((2, nPady))*(-1),
                                Qs[2],
                                np.ones((2, nPady))*(-1),
                                Qs[3],
                                ), axis=0)

    row_2 = np.concatenate((Qs[4],
                                np.ones((2, nPady))*(-1),
                                Qs[5],
                                np.ones((2, nPady))*(-1),
                                Qs[6],
                                np.ones((2, nPady))*(-1),
                                Qs[7],
                                ), axis=0)

    full = np.concatenate((row_2, np.ones((row_1.shape[0], 2))*(-1), row_1), axis=1)
    row_1T = np.concatenate((Ts[0],
                                np.ones((2, nPady))*(-1),
                                Ts[1],
                                np.ones((2, nPady))*(-1),
                                Ts[2],
                                np.ones((2, nPady))*(-1),
                                Ts[3],
                                ), axis=0)

    row_2T = np.concatenate((Ts[4],
                                np.ones((2, nPady))*(-1),
                                Ts[5],
                                np.ones((2, nPady))*(-1),
                                Ts[6],
                                np.ones((2, nPady))*(-1),
                                Ts[7],
                                ), axis=0)

    fullT = np.concatenate((row_2T, np.ones((row_1T.shape[0], 2))*(-1), row_1T), axis=1)
    fullT[full <=0] = None
    full[full <= 0] = None

    cur_time = int(time.time())

    fname = f"./data/Q_{cur_time}.npy"
    fnameT = f"./data/T_{cur_time}.npy"
    np.save(fname, full)
    np.save(fnameT, fullT)

if __name__ == "__main__":
    daq = DAQ("./")
    mapping = Mapping("./")
    asyncio.run(loop())
