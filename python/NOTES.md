# Synaptic conductance.

$$ \tau_1 \tau_2 \frac{d^2g}{dt^2} + (\tau_1+\tau_2)\frac{dg}{dt} + g = \bar{g} x(t) $$

Under zero initial conditions, we take the Laplace tranforms:

$$ \left(\tau_1\tau_2 s^2 + (\tau_1+\tau_2)s+1\right)G(s) = \bar{g}X(s)$$

$$\frac{G(s)}{X(s)} = \frac{\bar{g}}{\tau_1\tau_2 s^2 + (\tau_1+\tau_2)s+1}$${#eq:1}

## Alpha synapse

When $\tau_1=\tau_2=\tau$, 

$$T_\alpha(s)=\frac{G(s)}{X(s)} = \frac{\bar{g}}{\tau^2 s^2 + 2\tau s+1} 
                    = \frac{\bar{g}}{(s+\tau)^2}$${#eq:alpha_transfer}

The transfer function for alpha synapse (@eq:alpha_transfer) has the following
impulse response $\frac{t}{\tau^2}e^{-t/\tau}$.


Taking inverse laplace transform,

$$g(t) = \frac{t}{\tau^2}e^{-t/\tau} \circledast x(t)$${#eq:alpha} where
$\circledast$ is convolution.
