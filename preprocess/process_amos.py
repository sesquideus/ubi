#!/usr/bin/env python

import pandas as pd
import numpy as np
import argparse
import sys

from coord import sph_to_cart


class Processor():
    def __init__(self):
        self.argparser = argparse.ArgumentParser("Preprocess AMOS database file")
        self.argparser.add_argument('infile', type=argparse.FileType('r'), help="input file")
        self.argparser.add_argument('outfile', type=argparse.FileType('w'), default=sys.stdout, nargs='?', help="output file")
        self.args = self.argparser.parse_args()

    def load(self):
        self.indf = pd.read_csv(self.args.infile, sep='\t')

    def process(self):
        self.data = pd.DataFrame()

        timestamp_columns = ['year', 'month', 'day', 'hour', 'min', 'sec']
        self.data['time'] = pd.to_datetime(self.indf[timestamp_columns].rename(columns={'min': 'minute', 'sec': 'second'}))
        self.data['speed'] = self.indf['geo.v[km/s]'] * 1000
        self.data['ra'] = self.indf['rad.geo.ra[deg]']
        self.data['dec'] = self.indf['rad.geo.dec[deg]']
        self.data['vx'], self.data['vy'], self.data['vz'] = sph_to_cart(np.radians(self.data.dec), np.radians(self.data.ra), self.data.speed)
        self.data['mass'] = self.indf['mass.cep[g]'] / 1000
        self.data['absmag'] = self.indf[''
        self.data['sunlon'] = self.indf['L.sun[deg]']
        self.data = self.data[(self.data['mass'] > -np.inf) & (self.data['speed'] < 75000)]
        print(self.data)
        print(np.sqrt(np.var(self.data['speed'])))

    def save(self):
        self.data.to_csv(self.args.outfile, sep='\t', index=False)


processor = Processor()
processor.load()
processor.process()
#processor.save()
