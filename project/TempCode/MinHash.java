import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Random;
import java.util.Set;
import java.util.TreeSet;

public class MinHash implements Serializable {

    public static double jaccardIndex(final Set<Integer> s1, final Set<Integer> s2) {

        Set<Integer> intersection = new HashSet<Integer>(s1);
        intersection.retainAll(s2);

        Set<Integer> union = new HashSet<Integer>(s1);
        union.addAll(s2);

        if (union.isEmpty()) {
            return 0;
        }

        return (double) intersection.size() / union.size();
    }
    
  //Compute the exact similarity index between two objects or signatures
    public static double similarityIndex(final int[] s1, final int[] s2) {
    	int elementNumber1 = s1[s1.length - 2]; //number of element in this vector
    	int elementSize1 = s1[s1.length - 1]; //size of element in this vector
    	int elementNumber2 = s2[s2.length - 2];
    	int elementSize2 = s2[s2.length - 1];
    	
    	int[] s3 = new int[s1.length + 2];
    	int[] s4 = new int[s2.length + 2];
    	for(int i = 0; i < s3.length; i++){
    		if(i < s1.length - 2)
    			s3[i] = s1[i];
    		else
    			s3[i] = 1;
    	}
    	
    	for(int i = 0; i < s2.length - 2; i++){
    		s4[i] = s2[i];
    	}
    	
    	if( lastPowerOf2(elementNumber1) == lastPowerOf2(elementNumber2) ){
    		s4[s2.length - 2] = 1;
    	}else{
    		s4[s2.length - 2] = 0;
    	}
    	
    	if( lastPowerOf2(elementSize1) == lastPowerOf2(elementSize2) ){
    		s4[s2.length - 1] = 1;
    	}else{
    		s4[s2.length - 1] = 0;
    	}
    	
    	if( nextPowerOf2(elementNumber1) == nextPowerOf2(elementNumber2) ){
    		s4[s2.length] = 1;
    	}else{
    		s4[s2.length] = 0;
    	}
    	
    	if( nextPowerOf2(elementSize1) == nextPowerOf2(elementSize2) ){
    		s4[s2.length + 1] = 1;
    	}else{
    		s4[s2.length + 1] = 0;
    	}
    	
//    	System.out.print("s3:");
//		for(int i = 0; i < s3.length; i++ ){
//			System.out.print(s3[i] + " ");
//		}
//		System.out.println();
//		
//		System.out.print("s4:");
//		for(int i = 0; i < s4.length; i++ ){
//			System.out.print(s4[i] + " ");
//		}
//		System.out.println();

//    	System.out.println("last:" + i);
//    	System.out.println("next:" + j);
        return jaccardIndex(convert2Set(s1), convert2Set(s2));
    }

    public static int lastPowerOf2(int num){
    	return (int) (Math.log(num)/Math.log(2));
    }
    
    public static int nextPowerOf2(int num){
    	return (int) (Math.log(num)/Math.log(2)) + 1;
    }
    
    //Compute the exact jaccard index between two objects or signatures
    public static double jaccardIndex(final int[] s1, final int[] s2) {
    	
        return jaccardIndex(convert2Set(s1), convert2Set(s2));
    }

    
    //Covert the object by deleting all 0's, in order to save space
    public static Set<Integer> convert2Set(final int[] array) {
        Set<Integer> set = new TreeSet<Integer>();
        for (int i = 0; i < array.length; i++) {
            if (array[i]==1) {
                set.add(i);
            }
        }
        return set;
    }

    //Signature size.
    private int sig_size;

    //Random a and b coefficients for the random hash functions.
    private long[][] hash_coefs;

    //Object size (is also the size of vectors if the sets are provided

    private int object_size;

    //Initializes hash functions to compute MinHash signatures for objects.
    public MinHash(final int size, final int object_size) {
        init(size, object_size, new Random());
    }
    
    //Compute the signature
    public final int[] signature(final int[] object) {
        return signature(convert2Set(object));
    }

    //Computes the signature for this objects.
    public final int[] signature(final Set<Integer> set) {
        int[] sig = new int[sig_size];

        for (int i = 0; i < sig_size; i++) {
            sig[i] = Integer.MAX_VALUE;
        }

        // Loop over true values, instead of loop over all values of objects to speedup computation
        final List<Integer> list = new ArrayList<Integer>(set);
        Collections.sort(list);

        for (final int r : list) {

            // If c has 1 in row r, then for each i, set SIG(i, c) to the smaller of the current value of SIG(i, c) and hi(r)
            for (int i = 0; i < sig_size; i++) {
                sig[i] = Math.min(sig[i], h(i, r));
            }
        }

        return sig;
    }

    //Computes an estimation of Jaccard similarity (the number of elements incommon) between two signatures
    public final double similarity(final int[] sig1, final int[] sig2) {
        double sim = 0;
        for (int i = 0; i < sig1.length; i++) {
            if (sig1[i] == sig2[i]) {
                sim += 1;
            }
        }

        return sim / sig1.length;
    }

    //Compute hash function coefficients using provided Random.
    private void init(final int size, final int object_size, final Random r) {

        this.object_size = object_size;
        this.sig_size = size;

        // h = (a * x) + b, a and b should be randomly generated
        hash_coefs = new long[sig_size][2];
        for (int i = 0; i < sig_size; i++) {
            hash_coefs[i][0] = r.nextInt(object_size); // a
            hash_coefs[i][1] = r.nextInt(object_size); // b
        }
    }

    // Computes hi(x) as (a_i * x + b_i) % objects_size.
    private int h(final int i, final int x) {
        return (int)((hash_coefs[i][0] * (long) x + hash_coefs[i][1]) % object_size);
    }
}