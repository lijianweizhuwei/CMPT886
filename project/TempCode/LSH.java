import java.io.Serializable;

public abstract class LSH implements Serializable {

    protected static final long LARGE_PRIME =  433494437;
    private static final int DEFAULT_BANDS = 3;
    private static final int DEFAULT_BUCKETS = 10;

    private int bands = DEFAULT_BANDS;
    private int buckets = DEFAULT_BUCKETS;

    //Instantiates a LSH instance with s  bands and b buckets (per band)
    public LSH(final int stages, final int buckets) {
        this.bands = stages;
        this.buckets = buckets;
    }

    //Hash a signature.
    public final int[] hashSignature(final int[] signature) {

    	//signature is for each object
        int[] hash = new int[bands];
        
        // Number of rows per stage
        int rows = signature.length / bands;
        
        for (int i = 0; i < signature.length; i++) {
            int stage = Math.min(i / rows, bands - 1);
            hash[stage] = (int)
                    ((hash[stage] + (long) signature[i] * LARGE_PRIME)
                    % buckets);

        }

        return hash;
    }
}