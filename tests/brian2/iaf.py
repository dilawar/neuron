import matplotlib as mpl
import matplotlib.pyplot as plt
try:
    mpl.style.use( ['seaborn-paper'] )
except Exception as e:
    print(e)
    print(mpl.style.available)
import numpy as np
from brian2 import *

N           = 1             # Neurons.
runtime_    = 0.2             # in second.
Cm          = 100e-12*farad
Rm          = 100e6*ohm
Erest       = -65*mV

# excitatory synapses.
gEbar, tauE = 10*nS, 1*ms
gIbar, tauI = 0*nS, 100*ms

VeSyn       = 0.0*mV    # excitatory reversal potential.
ViSyn       = -70*mV    # inhibitory reversal potential.

def stim(freq=20):
    global runtime_, N
    index, events = [], []
    index = range(N)
    times = np.zeros(N) * second
    T = 1/freq*second
    return PoissonGroup(N, 20*Hz)
    #  return SpikeGeneratorGroup(N, index, times, period=T)


def main():
    # input
    print( f"[INFO ] Tau is {Cm*Rm} sec" )
    P = stim()
    G = NeuronGroup(N
            , '''
                dv/dt=-(v-Erest)/(Rm*Cm) - (inject/Cm): volt
                inject =  gE*(v-VeSyn) + gI*(v-ViSyn) : amp
                gE : siemens
                gI : siemens
                '''
            , threshold = 'v>-50*mV'
            , reset = 'v=-65*mV'
            , method = 'rk4'
            )
    G.v = -65*mV

    SE = Synapses(P, G
            , model = '''
                prevT : second
                g=gEbar*exp(-t/tauE): siemens 
            '''
            , on_pre='''
                gE = g
                prevT = t
                '''
            )
    SE.connect('i==j')

    # inhibitory synapse.
    SI = Synapses(P, G
            , model = 'preT : second'
            , on_pre='''
                gI=gIbar*exp(-(t-0*second)/tauI)
                preT = t
            '''
            , delay = 0*ms
            )
    SI.connect('i==j')

    MGspike = SpikeMonitor(G) #, variables=['v'])
    MPspike = SpikeMonitor(P) #, variables=['x'])
    MGv = StateMonitor(G, variables=['v', 'gE', 'gI', 'inject'], record=True)
    MPopFreq = PopulationRateMonitor(G)

    run( runtime_ *second)

    plt.figure(figsize=(6,6))
    ax1 = plt.subplot(511)
    plt.plot(MGv.t, MGv.v[0:5].T, alpha=0.5)
    plt.xlabel('t')
    plt.ylabel('v')

    ax2 = plt.subplot(512, sharex=ax1)
    ax2.plot(MPspike.t, MPspike.i, 'k.')

    ax3 = plt.subplot(513, sharex=ax1)
    ax33 = ax3.twinx()
    ax3.plot(MGspike.t, MGspike.i, 'k.', markersize=3, alpha=0.5)
    smoothF = MPopFreq.smooth_rate(width=10*ms)
    ax33.plot(MPopFreq.t, smoothF)
    plt.axhline(np.mean(smoothF)/Hz, linestyle='-.', alpha=0.2, lw=2)
    plt.legend(framealpha=0.0, loc=1)
    plt.ylabel('Hz')
    
    ax4 = plt.subplot(514, sharex=ax1)
    ax4.plot(MGv.t, MGv.gE[0:5].T, alpha=0.5, label='gE')
    ax4.plot(MGv.t, MGv.gI[0:5].T, alpha=0.5, label='gI')
    plt.legend()

    ax = plt.subplot(515)
    ax.plot(MGv.t, MGv.inject[0:5].T, alpha=0.5, label='i')

    plt.tight_layout()
    plt.savefig( f'{__file__}.png')

if __name__ == '__main__':
    main()
