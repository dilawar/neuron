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

spikes_ = [1e-3, 3e-3, 5e-3]
tau1 = 1e-3
tau2 = 1e-3
gb   = 1e-9

def spike(t):
    global spikes_
    if not spikes_:
        return 0.0
    if t >= spikes_[0]:
        print(t)
        spikes_.pop(0)
        return 10
    return 0.0


def f(t, y):
    return [y[1], (-y[0] - (tau1+tau2)*y[1] + gb*spike(t))/tau1/tau2]

sol = solve_ivp(f, (0, 2e-2), (0.0,0.0), max_step=0.2e-3) #t_eval = tt)
plt.plot(sol.t, sol.y[0,:], '-o')
plt.ylim(0, gb)
plt.show()


