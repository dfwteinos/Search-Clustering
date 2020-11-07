# Search-Clustering

Implementation of famous Data-Science **Search** and **Clustering Algorithms**, such as: 

*   [Locality-Sensitive-Hashing](https://en.wikipedia.org/wiki/Locality-sensitive_hashing)
*   [Randomized-Projections](https://arxiv.org/pdf/1612.07405.pdf)
*   [KNN](https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm)
*   [Range Searching](https://en.wikipedia.org/wiki/Range_searching)
*   [Lloyd's Algorithm](https://en.wikipedia.org/wiki/Lloyd%27s_algorithm) 
*   [K-Means](https://en.wikipedia.org/wiki/K-means_clustering) 
*   [K-Medians](https://en.wikipedia.org/wiki/K-medians_clustering)
*   [Silhouette Evaluation](https://en.wikipedia.org/wiki/Silhouette_(clustering))

## Dataset:

For our dataset, we used the [MNIST Database](http://yann.lecun.com/exdb/mnist/). To be more specific, in our project, inside the **data** directory, we have 2 files.
*   Training Dataset : **train-images-idx3-ubyte** 
*   Query Dataset : **t10k-images-idx3-ubyte**

The training dataset contains 60.000 images. Each image is made up from a product of constant rows and columns (`28*28`), 784 Pixels in total. Each pixel is a number from 0 up to 255. 
The query dataset contains the same material, but reduced to 10.000 images.

In order to train our Algorithms and fill-up all the hashing tables, we obviously used the training dataset.
Then, we used the query dataset in order to test the perfomance of our Algorithms.

It's better to conceptually split this project into 2 smaller ones:
 
## Search:

For Searching, we implemented 2 famous Algorithms as we mentioned before:

*   **Locality Sensitive Hashing**
*   **Randomized Projections using Hypercube** 

The main idea here is that given a training dataset (in our occasion: **data**/train-images-idx3-ubyte), we "trained" our algorithms, meaning that we filled up the buckets of our hashtable evenly (or not, depending at a hyperparameter **w**, which regulates the distribution).
Then, for each image in the query dataset, we searched the K-Near(Similar) Neighbours of this image, using [Manhattan Distance](https://en.wikipedia.org/wiki/Taxicab_geometry) for LSH, and [Hamming Distance](https://en.wikipedia.org/wiki/Hamming_distance) for Hypercube.

### Compile and run:

* For *LSH*: 

If you want to have the full control of the parameters, simply type: `make` , and then: `./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν
<number of nearest> -R <radius>` . <radius> parameter, stands for R-Nearest Neighbours.

But, if you want to run the LSH Searching Algorithm using default parameters, simply type: `make lshdef`.

* For *Hypercube*:

Similar, if you want to have the full control of the parameters, simply type: `make`, and then: `./cube –d <input file> –q <query file> –k <int> -M <int> -probes <int> -ο
<output file> -Ν <number of nearest> -R <radius>`.

But, if you want to run the Hypercube Searching Algorithm using default parameters, simply type: `make cubedef`.

## Clustering:

For clustering, we implemented among others, 3 famous Algorithms:

*   **Lloyd's Algorithm**
*   **LSH Range-Search**
*   **Hypercube Range-Search**

The main idea here, is that at fist, we initialize our **centroids**, using K-Means Algorithm. Then, for a specific number of iterations (or until our centroids position remains the same), we perform clustering with the 3 methods we mentioned before. Then, we update the centroids position using K-Medians Algorithm. Final, we evaluate our clustering for the method that we choose, using the Silhouette.

### Compile and run:

* For *Lloyd's* : 

In order to run the Lloyd's algorithm, using the given datasets, simply type: `make clucla`

* For *LSH-RS* :

In order to run the LSH-RS algorithm, using the given datasets, simply type: `make clulsh`

* For *HPC-RS*:

In order to run the HPC-RS algorithm, using the given datasets, simply type: `make cluhpc`

Alternative, if you want to give different datasets and choose your pref. algorithm, simply type: `make`, and then: `./cluster -i  <input file> –c <configuration file> -o <output file> -complete
<optional> -m <method: Classic OR LSH or HyperCube>` .

## Further informations

This project contains also a detailed README inside the *README* dir.  
For this big project, I've collaborated with the fellow partner: [Evgenia Kalyva](https://github.com/Jenn4K)

*This project is part of the course: Software Development for Algorithmic Problems , Winter of 2020. University of Athens, DiT.*
