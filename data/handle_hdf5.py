from __future__ import print_function
import h5py
import sys
import numpy as np


def load_csc(mat_path, save_path):
    obj = h5py.File(mat_path, 'r')
    csc = obj['Problem']['A']
    # nnz
    data = np.array(csc['data'])
    # col_idx
    jc = np.array(csc['jc'])
    # row number
    ir = np.array(csc['ir'])
    #write to coo
    nnz_count = jc[-1]
    assert len(data)==nnz_count, "nnz count error"
    assert len(ir)==nnz_count, "row_number count error"

    col = 0
    with open(save_path, 'w') as f:
        for idx in range(nnz_count):
            val = data[idx]
            row = ir[idx]
            while(jc[col+1]<=idx):
                col += 1
            f.write(str(row)+' '+str(col)+'\n')

if __name__ == '__main__':
    load_csc(sys.argv[1], sys.argv[2])
