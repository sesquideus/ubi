import argparse
import numpy as np
import pandas as pd

from pathlib import Path
from dotmap import DotMap
from matplotlib import pyplot as plt
import matplotlib

from utils.utils import ReadableDir, WriteableDir

import heatmap

plt.rcParams["font.family"] = "Minion Pro"
plt.rcParams["font.size"] = 24
matplotlib.rcParams['mathtext.it'] = 'Garamond Math:italic'
matplotlib.rcParams['mathtext.bf'] = 'Garamond Math:bold'
matplotlib.use('Agg')


class Limits():
    def __init__(self, min, max, count):
        self.min = min
        self.max = max
        self.count = count

    def value(self, index):
        f = (index / self.count)
        return (1 - f) * self.min + f * self.max


class GridSky():
    def __init__(self, args):
        self.dec    = Limits(-90, 90, args.dec)
        self.ra     = Limits(0, 360, args.ra)
        self.v      = Limits(10, 70, args.v)
        self.lnm    = Limits(0, 0, args.lnm)
        self.lsun   = Limits(0, 359, args.lsun)
        self.extent = [self.ra.min, self.ra.max, self.dec.min, self.dec.max]


class HeatmapSky(heatmap.Heatmap):
    def __init__(self):
        super().__init__()
        self.outdir = Path(self.args.outdir) if self.args.outdir else Path('.')

    def add_arguments(self):
        self.argparser.add_argument('outdir', action=WriteableDir, help="Output directory", default='.')
        self.cmap_arg = self.argparser.add_argument('-c', '--cmap', type=str, default='viridis')
        self.dec_arg  = self.argparser.add_argument('-d', '--dec', type=int, default=1)
        self.ra_arg   = self.argparser.add_argument('-r', '--ra', type=int, default=1)
        self.v_arg    = self.argparser.add_argument('-v', '--v', type=int, default=1)
        self.lnm_arg  = self.argparser.add_argument('-m', '--lnm', type=int, default=1)
        self.lsun_arg = self.argparser.add_argument('-l', '--lsun', type=int, default=1)

    def load_kde(self):
        self.grid = GridSky(self.args)
        self.extent = [self.grid.ra.min, self.grid.ra.max, self.grid.dec.min, self.grid.dec.max]
        self.kde = np.fromfile(self.kdefile, sep='', dtype=float)
        self.kde = self.kde.reshape(self.args.dec, self.args.ra, self.args.v, self.args.lnm, self.args.lsun)

    def load_points(self):
        if self.args.points:
            self.points = pd.read_csv(self.args.points, sep='\t')
            self.points = self.points[self.points['mass'] > 0]
        #    self.points = self.points[(self.points['ra'] >= ralo) & (self.points['ra'] <= rahi) & (self.points['dec'] >= declo) & (self.points['dec'] <= dechi)]

    def video_frames(self):
        self.kde = np.sum(self.kde, axis=(2, 3))

        for frame in range(0, self.args.lsun):
            fig = plt.figure(figsize=[16, 9])
            ax = fig.subplots(1, 1)
            ax.set_frame_on(False)
            ax.grid(alpha=1.0)
#            ax.axis('off')

            ax.imshow(self.kde[:, :, frame], cmap=self.args.cmap, vmin=0, vmax=10, extent=self.extent, origin='lower')
            if self.args.points:
                ax.scatter(self.points['ra'], self.points['dec'], s=0.5, c=self.points['sunlon'] / 360, cmap='hsv')

            fig.patch.set_facecolor('black')
            plt.tight_layout(pad=0)
            fig.savefig(self.outdir / f'{frame:03d}.png', dpi=120, pad_inches=0, format='png')
            print(f"[{frame:03d}]", end='\n' if frame + 1 == self.args.lsun else ' ', flush=True)
            plt.close('all')

        print(f"Video frames saved to {self.outdir}")

    def time_frames(self):
        self.kde = np.sum(self.kde, axis=(2, 3))
        self.total = np.sum(self.kde, axis=(0, 1)) / 1000
        self.kde = np.log(1 + self.kde)


        for frame in range(0, 360):
            fig = plt.figure(figsize=[16, 10])
            ax = fig.subplots(2, 1, gridspec_kw={'height_ratios': [5, 1]})
            ax[0].set_aspect(1)
            ax[0].grid(alpha=0.5, color='gray')
            #ax.set_title(f'The PDF for the Perseid meteor shower, λ = {frame:.1f}°')
            ax[0].set_title(f'The global AMOS meteor PDF, λ = {frame:.0f}°')
            ax[0].set_xlabel('Right ascension / °')
            ax[0].set_ylabel('Declination / °')
            ax[0].set_xlim(self.grid.ra.max, self.grid.ra.min)
            ax[0].set_ylim(self.grid.dec.min, self.grid.dec.max)
            ax[0].xaxis.set_major_locator(matplotlib.ticker.MultipleLocator(base=30))
            ax[0].yaxis.set_major_locator(matplotlib.ticker.MultipleLocator(base=15))

            im = ax[0].imshow(self.kde[:, :, frame], cmap=self.args.cmap, vmin=0, vmax=10, extent=self.extent, origin='lower')
            fig.colorbar(im, ax=ax, shrink=0.8, aspect=30*0.8, label='ln(1 + Â)', pad=0.01)

            ax[1].plot(self.total)
            ax[1].set_xlim(0, 360)

            if self.args.points:
                points = self.points#[np.abs(self.points.sunlon - frame) < 0.5]
                ax[0].scatter(points['ra'], points['dec'], s=1, c=points['sunlon'] / 360, cmap='hsv', linewidths=0)

            fig.patch.set_facecolor('white')
            plt.tight_layout(rect=(0, 0, 1.08, 1))
            fig.savefig(self.outdir / f'{frame:03d}.png', dpi=200, format='png')
            print(f"[{frame:03d}]", end='\n' if frame + 1 == self.args.lsun else ' ', flush=True)
            plt.close('all')

        print(f"Video frames saved to {self.outdir}")
