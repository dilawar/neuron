"""test_synapse_ode.py: 

Test ODE equations of synapse.

"""
    
__author__           = "Dilawar Singh"
__copyright__        = "Copyright 2017-, Dilawar Singh"
__version__          = "1.0.0"
__maintainer__       = "Dilawar Singh"
__email__            = "dilawars@ncbs.res.in"
__status__           = "Development"

import math
import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import solve_ivp

tau1 = 1e-3
tau2 = 1e-3
gb   = 1e-9

spks = [0, 1e-3, 5e-3]
spikes_ = spks[:]

def spike(t, spikes):
    if not spikes:
        return 0.0
    if t >= spikes[0]:
        print(f'@{t}')
        spikes.pop(0)
        return 1.0
    return 0.0

def alpha1(t):
    spikes1_ = spks.copy()
    tau = tau1
    g = np.zeros_like(t)
    for ts in spikes1_:
        tt = t - ts
        g1 = gb*tt/tau*np.exp(-tt/tau)
        g1[g1 < 0 ] = 0
        g += g1
    return g

def alpha(t):
    tau = tau1
    return gb * t / tau * np.exp(-t/tau)

def damped_oscillator(t, y, zeta=1.0):
    """damped_oscillator

    System driven by external force Fext.

        d^2x(t)         dx(t)
        -------  + 2ζω0 -----  + ω0 = Fext(t)
          dt^2           dt 


    :param t:
    :param y:
    :param zeta:
    """
    s = 1.0
    tau = tau1
    return [y[1], (-y[0]-2*tau*y[1]+gb*s*spike(t, spikes_))/tau/tau]

def ff(t, y):
    tau = tau1
    if t == 0:
        return 0.0
    return [y[1], (-y[0]*tau - gb/t)/tau/tau]

def ode1(t, y):
    tau = tau1
    return -y/tau + math.exp(-t/tau)*gb/tau

plt.subplot(211)
for dt in [1e-3, 1e-4, 1e-5]:
    spikes_ = spks.copy()
    sol = solve_ivp(ode1, (0,1.2e-2), (0.0,), max_step=dt) 
    plt.plot(sol.t, sol.y[0,:], label = f'ode dt={dt:g}')

plt.legend()

plt.subplot(212)
plt.plot(sol.t, alpha(sol.t), lw=2, color='black', label='alpha')
plt.legend()

plt.show()
