import argparse
import numpy as np
import pandas as pd

from pathlib import Path

from matplotlib import pyplot as plt
import matplotlib

from utils import ReadableDir, WriteableDir
import heatmap

plt.rcParams["font.family"] = "Minion Pro"
matplotlib.rcParams['mathtext.it'] = 'Garamond Math:italic'
matplotlib.rcParams['mathtext.bf'] = 'Garamond Math:bold'
matplotlib.use('Agg')


class HeatmapMollweide(heatmap.Heatmap):
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
        self.kde = np.fromfile(self.kdefile, sep='', dtype=float)
        self.kde = self.kde.reshape(self.args.dec, self.args.ra, self.args.v, self.args.lnm, self.args.lsun)
        self.kde = np.log(1 + self.kde)

    def load_points(self):
        if self.args.points:
            self.points = pd.read_csv(self.args.points, sep='\t')
            self.points = self.points[self.points['mass'] > 0]
            self.points['ra'] = (self.points['ra'] + 180) % 360 - 180

    def time_frames(self):
        self.kde = np.sum(self.kde, axis=(2, 3))
        for frame in range(0, self.args.lsun):
            fig = plt.figure(figsize=[8, 5])
            ax = fig.subplots(1, 1, subplot_kw={'projection': 'mollweide'})
            ax.grid(alpha=0.5, color='black')
            ax.set_title(f'The global PDF, λ = {frame:.1f}°')
            ax.set_xlabel('Right ascension / °')
            ax.set_ylabel('Declination / °')

            lon = np.linspace(-89.75, 89.75, 360)
            lat = np.linspace(-179.75, 179.75, 720)
            lat, lon = np.meshgrid(lat, lon)

            im = ax.pcolormesh(lat, lon, self.kde[:, :, frame], cmap=self.args.cmap, vmin=0, vmax=10, shading='auto')
            cb = fig.colorbar(im, ax=ax, shrink=0.9, aspect=20, label='ln(1 + Â)')

            if self.args.points:
                ax.scatter(np.radians(self.points['ra']), np.radians(self.points['dec']), s=0.5, c=self.points['sunlon'] / 360, cmap='hsv')

            fig.patch.set_facecolor('white')
            plt.tight_layout()
            fig.savefig(self.outdir / f'{frame:03d}.png', dpi=200, format='png')
            print(f"[{frame:03d}]", end='\n' if frame + 1 == self.args.lsun else ' ', flush=True)
            plt.close('all')

        print(f"Video frames saved to {self.outdir}")
