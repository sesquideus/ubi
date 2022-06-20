import argparse
from pathlib import Path

from utils.utils import ReadableDir, WriteableDir


class Heatmap():
    def __init__(self):
        self.argparser = argparse.ArgumentParser(
            description="KDE binary file visualizer"
        )
        self.argparser.add_argument('infile', type=argparse.FileType('r'), help="Binary input file")
        self.pts_arg = self.argparser.add_argument('--points', type=argparse.FileType('r'), help="Points TSV file")

        self.add_arguments()
        self.args = self.argparser.parse_args()

        self.kdefile = Path(self.args.infile.name)
        self.pointfile = Path(self.args.points.name) if self.args.points else None

        self.load_kde()
        self.load_points()
