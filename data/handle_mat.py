import scipy.io as sio
import sys
import numpy as np
import scipy
import scipy.sparse as sparse
def load_mat(mat_path, save_path):
    mat = sio.loadmat(mat_path)
    csc = mat['Problem']['A']
    coo = sparse.coo_matrix(csc[0][0])

    with open(save_path, 'w') as f:
        for (r,c,_) in zip(coo.row, coo.col, coo.data):
            f.write(str(r)+' '+str(c)+'\n')
    # nnz
    #data = np.array(csc.data)
    #jc = np.array(csc.jc)
    #ir = np.array(csc.ir)

if __name__ == '__main__':
    load_mat(sys.argv[1], sys.argv[2])
