#!/usr/bin/env python
"""plot_csv.py: 
"""
    
__author__           = "Dilawar Singh"
__copyright__        = "Copyright 2017-, Dilawar Singh"
__version__          = "1.0.0"
__maintainer__       = "Dilawar Singh"
__email__            = "dilawars@ncbs.res.in"
__status__           = "Development"

import matplotlib as mpl
import matplotlib.pyplot as plt
mpl.style.use( ['bmh'] )
import pandas as pd

def main(args):
    plt.figure(figsize=(8, 1.75*len(args.input_ptype)))
    for i, f in enumerate(args.input_ptype):
        fptype = 'line'
        if ':' in f:
            f, fptype = f.split(':')
        print( f"[INFO ] Plotting {f}:{fptype}" )
        ax = plt.subplot(len(args.input_ptype)//2+1, 2, i+1)
        df = pd.read_csv(f)
        df = df.dropna(axis=1) # drop column
        col = list(df.columns)
        col.remove('time')
        df.plot(x='time', y=col[0], kind=fptype, ax=ax)
        if  len(df.columns) > 2:
            df.plot(x='time', y=col[1], kind=fptype, secondary_y=True, ax=ax)
        ax.set_title(f, fontsize=9)

    plt.tight_layout()
    if args.output:
        plt.savefig(args.output)
    else:
        plt.show()


if __name__ == '__main__':
    import argparse
    # Argument parser.
    description = '''Parse multiple CSV files. Each file goes into subplot.'''
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('--input-ptype', '-i', nargs='+', required = True
            , help = 'Input files with optional plot type e.g. data.csv:area will '
                    ' plot area curve. Default is line type.'
            )
    parser.add_argument('--output', '-o'
        , required = False, help = 'Output file')
    class Args: pass 
    args = Args()
    parser.parse_args(namespace=args)
    main(args)

