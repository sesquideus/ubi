import argparse
import numpy as np
import pandas as pd
import matplotlib

from pathlib import Path
from matplotlib import pyplot as plt

import heatmap


class Heatmap2D(heatmap.Heatmap):
    def add_arguments(self):
        self.argparser.add_argument('outfile', type=argparse.FileType('w'), help="PNG output file")

        self.w_arg = self.argparser.add_argument('w', type=int)
        self.h_arg = self.argparser.add_argument('h', type=int)
        self.cmap_arg = self.argparser.add_argument('-c', '--cmap', type=str, default='viridis')
        self.cpts_arg = self.argparser.add_argument('-C', '--cpts', type=str, default='white')

    def load_kde(self):
        self.kde = np.fromfile(self.kdefile, sep='', dtype=float)
        self.kde = self.kde.reshape(self.args.h, self.args.w)
        self.kde = np.log(1 + self.kde)

    def load_points(self):
        if self.args.points:
            self.points = pd.read_csv(self.args.points, sep='\t')

    def video_frames(self):
        self.outfile = Path(self.args.outfile.name)
        fig = plt.figure(1, figsize=(10, 10))
        ax = fig.subplots(1, 1)
        ax.set_frame_on(False)
        ax.grid(color='black', linestyle='-', alpha=0.6)
        ax.xaxis.set_major_locator(matplotlib.ticker.MultipleLocator(base=0.22))
        ax.yaxis.set_major_locator(matplotlib.ticker.MultipleLocator(base=0.22))
        ax.set_xticklabels([])
        ax.set_yticklabels([])
        ax.set_aspect('equal')

        ax.imshow(self.kde, cmap=self.args.cmap, vmin=0, extent=[-2, 2, -2, 2], origin='lower')
        if self.args.points:
            ax.scatter(self.points['x'], self.points['y'], s=2, c=self.args.cpts)

        plt.tight_layout(pad=0)
        fig.savefig(self.outfile, dpi=100, pad_inches=0, format='png')
        plt.close('all')
