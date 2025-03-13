import argparse
import polars as pl


class Extractor():
    def __init__(self, modules: list = []):
        self.modules = modules

        self.channel_per_module = {
                "C1": [31, 36],
                "C2": [30, 37],
                "C3": [41, 46],
                "C4": [40, 47],
                }
        self.channels = []

        self.load_reference_info()
        for module in modules:
            self.process_files(module)

        self.df = pl.DataFrame({"VGAIN": self.vgains, **self.runs, **self.values, **self.amplitudes, **self.charge})
        print(self.df)
        self.df.write_csv("VGAIN_SNR.csv")

    def load_reference_info(self):
        df = pl.read_csv(
                "/eos/home-h/hvieirad/December2024run/info_led_scan.csv"
                )

        self.vgains = df['VGAIN'].to_list()
        self.values = {}
        self.charge = {}
        self.amplitudes = {}
        self.runs = {
                "C1": df['C1'].to_list(),
                "C2": df['C2_C3'].to_list(),
                "C3": df['C2_C3'].to_list(),
                "C4": df['C4'].to_list(),
                }

    def process_files(self, module):
        try:
            self.channels = list(map(str, self.channel_per_module[module]))
        except KeyError:
            raise Exception("Module not found good sir...")

        module_search = module
        if module == "C3" or module == "C2":
            module_search = "C2"

        for ch in self.channels:
            self.values["SNR_"+ch] = []
            self.charge["Q_"+ch] = []
            self.amplitudes["A_"+ch] = []
        for run in self.runs[module]:
            if run == '-':
                for ch in self.channels:
                    self.values["SNR_"+ch].append(-1)
                    self.charge["Q_"+ch].append(-1)
                    self.amplitudes["A_"+ch].append(-1)
                continue
            file = '/eos/home-h/hvieirad/December2024run/'\
                   f'SPE_{module_search}/run_{run}/'
            for ch in self.channels:
                try:
                    with open(f"{file}/sphe{ch}.txt") as f:
                        lines = f.readlines()
                        lines = [line.strip() for line in lines]
                        # last line
                        self.values["SNR_"+ch].append(float(lines[-1].split()[-1]))
                        self.charge["Q_"+ch].append(float(lines[-1].split()[0]))
                except Exception as error:
                    print(error)
                    print("No sphe file...")
                    self.values["SNR_"+ch].append(-1)
                    self.charge["Q_"+ch].append(-1)

                try:
                    with open(f"{file}/sphe_wvf_info_Ch{ch}.txt") as f:
                        line = f.readline().strip().split(": ")[1]
                        self.amplitudes["A_"+ch].append(float(line))
                except Exception as error:
                    print(error)
                    print("No sphe file...")
                    self.amplitudes["A_"+ch].append(-1)

if __name__ == "__main__":
    parse = argparse.ArgumentParser()
    parse.add_argument("-m", "--module", type=str, nargs="+", help="Module to analyze, optsions = C1, C2, C3 or C4",)
    args = vars(parse.parse_args())

    module = args['module']
    if not module:
        print("Processing all modules...")
        module = ["C1", "C2", "C3", "C4"]

    ex = Extractor(module)
