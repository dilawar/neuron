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

tau = 5e-4
gb   = 1e-9

spks = [0e-3, 3e-3, 5e-3, 10e-3, 50e-3]
spikes_ = spks[:]
dt_ = 1e-3

# approximate spikes.
spikeX, spikeY, spikeDt = [], [], 0.2e-3
for s in spikes_:
    spikeX += [s+n*spikeDt for n in range(-4,5)]
    spikeY += [0, 0, 0.0, 0.1, 1.0, 0.1, 0.0, 0, 0]
spikeF = interp1d(spikeX, spikeY, kind='slinear', fill_value='extrapolate' )

def spike(t, spikes):
    global dt_
    #  return spikeF(t)
    if not spikes:
        return 0.0
    if t >= spikes[0]:
        delay = t - spikes[0]
        print(f'@{t} after {delay/dt_:g}')
        spikes.pop(0)
        return 1.0
    return 0.0

def alpha(t):
    spikes1_ = spks.copy()
    g = np.zeros_like(t)
    for ts in spikes1_:
        tt = t - ts
        g1 = gb*tt/tau*np.exp(-tt/tau)
        g1[g1 < 0 ] = 0
        g += g1
    return g

def pure_alpha(t):
    return gb * t / tau * np.exp(-t/tau)

def damped_oscillator(t, y, zeta=1.0):
    """damped_oscillator
    System driven by external force Fext.
    """
    return [y[1], (-2*tau*y[1] - y[0] + gb*spike(t, spikes_))/tau/tau ]

tmax = 0.07
plt.figure(figsize=(12,6))
ax1 = plt.subplot(211)
ax2 = plt.subplot(212)

tvec = np.linspace(0, tmax, 1000)
refY = alpha(tvec)
ax1.plot(tvec, refY, '--', lw=1, color='black', label='alpha')
ax21 = ax2.twinx()
ax21.plot(tvec, refY, '--', lw=1, color='black', label='alpha')
ax1.legend()

for dt in [1e-4, 1e-5, 1e-6]:
    dt_ = dt
    print( f"[INFO ] Using dt={dt:g}" )
    spikes_ = spks.copy()
    sol = solve_ivp(damped_oscillator, (0, tmax), (0.0,0.0), max_step=dt)
    y = sol.y[0,:]
    ax2.plot(sol.t, y, label = f'ode dt={dt:g}. r={refY.mean()/y.mean():g}')

ax2.legend()
plt.tight_layout()
plt.savefig( f'{__file__}.png' )

