from brian2 import *

def main():
    N = 100

    # input
    #  P = NeuronGroup(N, 'dx/dt= 20*Hz : 1', threshold='x>1', reset='x=0')
    P = NeuronGroup(N, 'rates :Hz', threshold='rand()<rates*dt')
    P.rates = 20*Hz
    #  P = PoissonGroup(N, 20*Hz)

    Cm = 100e-12*farad
    Rm = 100e6*ohm

    print( f"[INFO ] Tau is {Cm*Rm} sec" )
    Erest = -65*mV

    # excitatory synapses.
    gEbar = 30 * nS
    gIbar = 20 * nS
    tauE = 10 * ms
    tauI = 30 * ms

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
            #  , method = 'rk4'
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

    SM = SpikeMonitor(G, variables=['v'])
    SN = SpikeMonitor(P) #, variables=['x'])
    M = StateMonitor(G, 'v', record=True)

    run(2*second)

    plt.subplot(211)
    #plot(M.t, M.v.T)
    plt.xlabel('t')
    plt.ylabel('v')

    plt.subplot(212)
    plot(SM.t, SM.i, '.')
    #plot(SN.t, SN.i, '.')
    plt.show()

if __name__ == '__main__':
    main()
