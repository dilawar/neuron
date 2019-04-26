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
from scipy.interpolate import interp1d

tau1 = 1e-3
tau2 = 1e-3
gb   = 1e-9

spks = [0e-3, 3e-3, 5e-3, 10e-3, 50e-3]
spikes_ = spks[:]

# approximate spikes.
spikeX, spikeY, spikeDt = [], [], 0.2e-3
for s in spikes_:
    spikeX += [s+n*spikeDt for n in range(-4,5)]
    spikeY += [0, 0, 0.0, 0.1, 1.0, 0.1, 0.0, 0, 0]
spikeF = interp1d(spikeX, spikeY, kind='slinear', fill_value='extrapolate' )

def spike(t, spikes):
    return spikeF(t)
    #if not spikes:
    #    return 0.0
    #if t >= spikes[0]:
    #    print(f'@{t}')
    #    spikes.pop(0)
    #    return 1.0
    #return 0.0

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

def pure_alpha(t):
    tau = tau1
    return gb * t / tau * np.exp(-t/tau)


def damped_oscillator(t, y, zeta=1.0):
    """damped_oscillator

    System driven by external force Fext.

        d^2x(t)         dx(t)
    ω^2 -------  + 2ζω  -----  + ωx = Fext(t)
         dt^2            dt 

    ω and ζ  can be computed using τ1 and τ2. 

    ω = 1/√τ₁τ₂ and ζ=0.5(τ₁+τ₂)ω

    :param t:
    :param y:
    :param zeta:
    """
    w = (tau1*tau2)**-0.5
    z = 0.5*(tau1+tau2)*w
    assert w > 0
    assert z >= 1.0
    tau = tau1
    return [y[1], -2*z*w*y[1] - w**2*y[0] + gb*2*w*w*spike(t, spikes_)]

tmax = 0.07
plt.figure(figsize=(12,6))
for dt in [1e-3, 3e-4, 5e-4, 1e-4, 5e-5]:
    spikes_ = spks.copy()
    sol = solve_ivp(damped_oscillator, (0, tmax), (0.0,0.0), max_step=dt)
    plt.plot(sol.t, sol.y[0,:], label = f'ode dt={dt:g}')
plt.plot(sol.t, alpha(sol.t), '--', lw=1, color='black', label='alpha')
plt.legend()

plt.show()
