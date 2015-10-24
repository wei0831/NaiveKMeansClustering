# Naive K Means Clustering
Simple implementation of K Means Clustering for Machine Learning.

## Pseudocode
```
Randomly select K number of dataset and use as centroids
While the centroids is not converged to fixed points
  Assign group to each dataset depends on the closest centroid
  Average the dataset in the same group to form new centroids
```

## Compile & Run
Require Visual Studio 2013 / 2015

- Run **_build_vs2013.bat** to generate VS2013 project
- Run **_build_vs2015.bat** to generate VS2015 project

## Screenshots
__Input: Initial Step__  
![STEP 0](/screenshots/STEP0.png?raw=true)

__Output__
Step 1:  
![STEP 1](/screenshots/STEP1.png?raw=true)
Step 2:  
![STEP 2](/screenshots/STEP2.png?raw=true)
Step 3:  
![STEP 3](/screenshots/STEP3.png?raw=true)
Step 4: Converged:  
![STEP 4](/screenshots/STEP4.png?raw=true)

## Reference
[http://stanford.edu/~cpiech/cs221/handouts/kmeans.html](http://stanford.edu/~cpiech/cs221/handouts/kmeans.html)

## Author
**[Jack Chang]** : [![endorse](https://api.coderwall.com/wei0831/endorsecount.png)](https://coderwall.com/wei0831)

[Jack Chang]: https://about.me/wei0831
