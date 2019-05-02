from brian2 import *



def main():
    P = PoissonGroup(1, 50*Hz)
    group = NeuronGroup(1, 'dv/dt=-v/(10*ms): volt'
            , threshold='v>-50*mV'
            , reset='v=-65*mV'
            )
    S = Synapses(P, group, on_pre='v+=0.1*mV')
    S.connect('i==j')

    monitor = SpikeMonitor(group)
    duration = 1*second
    run(duration)
    plot(group.v/mV, monitor.count / duration)
    xlabel('v (mV)')
    ylabel('Firing rate (sp/s)')
    show()

if __name__ == '__main__':
    main()
