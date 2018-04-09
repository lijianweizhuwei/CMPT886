public class LSHMinHash extends LSH {
    private final MinHash mh;

    public LSHMinHash(final int bands, final int buckets, final int object_size, final int permutationNumber) {
        super(bands, buckets);
        this.mh = new MinHash(permutationNumber, object_size);
    }

    // Calculate the signature
    public final int[] getSigs(final int[] object) {
    	return this.mh.signature(object);
    }
    
    // Calculate the LSH hash
    public final int[] hash(final int[] sigs) {
        return hashSignature(sigs);
    }
}