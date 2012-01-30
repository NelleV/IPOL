import numpy as np


def affinity_propagation(s, lamb=0.5, maxit=300):
    """
    Affinity propagation

    Parameters:
    -----------
    s: n*n ndmatrix

    lamb: float, optional
        between 0 and 1

    maxit: int, optional
        max number of iteration

    Returns:
    exemplar
    """

    assert lamb > 0
    assert lamb < 1

    exemplar = np.zeros((len(s), 1))
    availability = np.zeros(s.shape)
    responsability = np.zeros(s.shape)
    ind = np.arange(len(s))

    for i in range(maxit):
        old_responsability = responsability.copy()
        old_availability = availability.copy()
        AS = availability + s
        I = np.argmax(AS, axis=1)
        Y = AS[np.arange(len(s)), I]

        AS[ind, I[ind]] = - np.finfo(np.double).max
        Y2 = np.max(AS, axis=1)
        responsability = s - Y[:, np.newaxis]
        responsability[ind, I[ind]] = s[ind, I[ind]] - Y2[ind]
        responsability = (1 - lamb) * responsability + lamb * old_responsability

        # Compute availibity

        Rp = np.maximum(responsability, 0)
        Rp.flat[::len(s) + 1] = responsability[::len(s) + 1]

        availability = np.sum(Rp, axis=0)[np.newaxis, :] - Rp

        dA = np.diag(availability)
        availability = np.minimum(availability, 0)

        availability.flat[::len(s) + 1] = dA

        availability = (1 -lamb) * availability + lamb * old_availability

        message = (np.diag(availability) + np.diag(responsability))

        # FIXME Add convergence checks !
    I = np.where(message > 0)

    if I.size > 0:
        c = np.argmax(s[:, I], axis=1)

        c[I] = np.arange(I.size)
        for k in range(I.size):
            ii = np.where(c == k)[0]
            j = np.argmax(np.sum(s[ii[:, np.newaxis], ii], axis=0))
            I[k] = ii[j]
        c = np.argmax(s[:, I], axis=1)
        c[I] = np.arange(I.size)
        labels = I[c]
        cluster_centers_indices = np.unique(labels)
        labels = np.searchsorted(cluster_centers_indices, labels)
    else:
        labels = np.empty((len(s), 1))
        cluster_centers_indices = None
        labels.fill(np.nan)
    return cluster_centers_indices, labels
