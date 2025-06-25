import matplotlib
import matplotlib.pyplot as plt
matplotlib.use("TkAgg")
import numpy as np
from sklearn.decomposition import PCA

raw_data = []
clusters = []
fin_clusters = []


def get_dataset(data_path="dataset"):
    dataset = np.loadtxt(f"{data_path}.txt")
    return dataset


np_array = get_dataset()
np_clusters = get_dataset("clusters")
np_ch_clusters = get_dataset("changed_clusters")


def pca_vis():
    pca = PCA(n_components=2)
    pca.fit(np_array)

    pca_data = pca.transform(np_array)
    pca_clust = pca.transform(np_clusters)
    pca_ch_clust = pca.transform(np_ch_clusters)

    plt.scatter(pca_data[:, 0], pca_data[:, 1], label="Исходные данные")
    plt.scatter(pca_clust[:, 0], pca_clust[:, 1], color='red', label="Изначальное положение кластеров")
    plt.scatter(pca_ch_clust[:, 0], pca_ch_clust[:, 1], color='green', label="Изменненое положение кластеров",
                marker="x")
    plt.legend(fontsize="small")
    plt.show()



pca_vis()
