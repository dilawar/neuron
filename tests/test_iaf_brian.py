from brian2 import *



def main():
    N = 1
    P = PoissonGroup(N, 20*Hz)
    Cm = 100*pF
    Rm = 100e6*ohm

    print( f"[INFO ] Tau is {Cm*Rm} sec" )
    Erest = -65*mV

    # excitatory synapses.
    gEbar = 10 * nS
    gIbar = 20 * nS
    tauE = 10 * ms
    tauI = 20* ms

    VeSyn = 0.0*mV    # excitatory reversal potential.
    ViSyn = -70*mV    # inhibitory reversal potential.
    G = NeuronGroup(N
            , '''
                dv/dt=-(v-Erest)/(Rm*Cm) - (inject/Cm): volt
                inject =  gE*(v-VeSyn) + gI*(v-ViSyn) : amp
                gE : siemens
                gI : siemens
                '''
            , threshold = 'v>-50*mV'
            , reset = 'v=-65*mV'
            )
    G.v = -65*mV

    SE = Synapses(P, G
            , model = 'preT : second'
            , on_pre='''
                gE=gEbar*exp(-(t-preT)/tauE)
                preT = t
            '''
            )
    SE.connect('i==j')

    # inhibitory synapse.
    SI = Synapses(P, G
            , model = 'preT : second'
            , on_pre='''
                gI=gIbar*exp(-(t-preT)/tauI)
                preT = t
            '''
            )
    SI.connect('i==j')

    M = StateMonitor(G, 'v', record=True)
    SM = SpikeMonitor(G)

    run(10*second)
    plt.subplot(211)
    plot(M.t, M.v.T)
    plt.subplot(212)
    plot(SM.t, SM.i, '.')
    xlabel('t')
    ylabel('v')
    show()

if __name__ == '__main__':
    main()
