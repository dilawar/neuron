"""test_synapse_ode.py: 

Test ODE equations of synapse.

"""
    
__author__           = "Dilawar Singh"
__copyright__        = "Copyright 2017-, Dilawar Singh"
__version__          = "1.0.0"
__maintainer__       = "Dilawar Singh"
__email__            = "dilawars@ncbs.res.in"
__status__           = "Development"

import sys
import os
import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import solve_ivp

tau1 = 1e-3
tau2 = 1e-3
gb   = 1e-9

spks = [1e-3, 2e-3, 4e-3]
spikes_ = spks[:]

def spike(t, spikes):
    if not spikes:
        return 0.0
    if t >= spikes[0]:
        print(f'@{t}')
        spikes.pop(0)
        return 1.0
    return 0.0

def alpha(t):
    spikes1_ = spks.copy()
    tau = tau1
    g = np.zeros_like(t)
    for ts in spikes1_:
        tt = t - ts
        g1 = gb*tt/tau*np.exp(-tt/tau)
        g1[g1 < 0 ] = 0
        g += g1
    return g

def f(t, y):
    s = 1.0
    return [y[1], (-y[0]-(tau1+tau2)*y[1]+gb*s*spike(t, spikes_))/tau1/tau2]

for dt in [1e-6, 1e-5, 1e-4]:
    spikes_ = spks.copy()
    sol = solve_ivp(f, (0,1.2e-2), (0.0,0.0), max_step=dt) 
    plt.plot(sol.t, sol.y[0,:], label = f'ode dt={dt:g}')

plt.plot(sol.t, alpha(sol.t), lw=2, color='black', label='alpha')
plt.legend()

plt.show()
