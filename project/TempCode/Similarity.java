import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.Scanner;

public class Similarity {

	private static Scanner sc;
	private String filePath = "/Users/eric/grad/cmpt886/project/data.txt";
	private String perfFilePath = "/Users/eric/grad/cmpt886/project/performance.txt";
	private BufferedReader reader;
	private static int bufferSize = 5*1024*1024; // Read with buffer size 5M
	private static int rowNumber = -1; 
	private static int colunmNumberWithType = -1;
	private static int colunmNumber = -1;
	private static int p = -1; // Number of permutations
	private static int buckets = -1; // Number of buckets per band
	private static int bands = -1; // Number of bands
	
	private static double weightOfA = -1; 
	private static double weightOfB = -1; 
	private static double weightOfC = -1; 
	
	private static int SWAR = 0; 
	private static int TypeLegalization = 1;
	private static int Unchanged = -1;
	
	private int[][] dataMatrixWithTypeContain3; // The data matrix(transposed)
	private double[][] dataMatrixWithTypeAndOptPerf; // The data matrix(transposed)
	private int[][] dataMatrix; // The data matrix(transposed)
	private double[][] perfData; // The performance data matrix(transposed)
	private static int perfSize = 3;
	private static int[][] hashes; // The hash value matrix(transposed)
	private static int[][] sigs; // The signature matrix(transposed)
	private static double optThreshold = 0.0;
	private static double optTPRate = 0.0;
	private static int simPairWithOptThreshold = 0;
	private static int simPairWithOptThresholdAndSameType = 0;
	
	private String newInstrParas;
	
	public static void main(String[] args) throws Exception {
		
		sc = new Scanner(System.in); 
		System.out.println("Input the number of parameters："); 
		colunmNumberWithType = Integer.parseInt(sc.nextLine());
		colunmNumber = colunmNumberWithType - 1;
	    System.out.println("Input the number of objects："); 
	    rowNumber = Integer.parseInt(sc.nextLine());
	    
//	    System.out.println("Input the number of permutations："); 
//	    p = Integer.parseInt(sc.nextLine());
//	    System.out.println("Input the number of bands："); 
//	    bands = Integer.parseInt(sc.nextLine());
//	    System.out.println("Input the number of buckets："); 
//	    buckets = Integer.parseInt(sc.nextLine()); 

	    p = 100;
	    bands = 25;
	    buckets = 4; 
	    
	    sc = new Scanner(System.in); 
		System.out.println("The weight of A："); 
		weightOfA =  Double.parseDouble(sc.nextLine());
		System.out.println("The weight of B："); 
		weightOfB =  Double.parseDouble(sc.nextLine());
		System.out.println("The weight of C："); 
		weightOfC =  Double.parseDouble(sc.nextLine());

		new Similarity();
	}
	
	public Similarity() throws Exception{

		dataMatrixWithTypeContain3 = new int[rowNumber][colunmNumberWithType]; // The data matrix(transposed)
		dataMatrixWithTypeAndOptPerf = new double[rowNumber/3][colunmNumberWithType + 1]; // The data matrix(transposed)
		dataMatrix = new int[rowNumber/3][colunmNumber]; // The data matrix(transposed)
		perfData = new double[rowNumber][perfSize]; // The performance data matrix(transposed)
		hashes = new int[rowNumber][bands]; // The hash value matrix(transposed)
		sigs = new int[rowNumber][p]; // The signature matrix(transposed)
		
		readFile();
		calculateWithCertainWeight(weightOfA, weightOfB, weightOfC);
		
        LSHMinHash lsh = new LSHMinHash(bands, buckets, colunmNumber, p); 
        for (int i = 0; i < rowNumber/3; i++) {
        	int[] object = dataMatrix[i];
        	sigs[i] = lsh.getSigs(object); // Compute the signature for each object
            hashes[i] = lsh.hash(sigs[i]); // Compute the LSH hash value of each signature
        }

        for( Double tempThreshold = 0.1; tempThreshold <= 0.9; tempThreshold = tempThreshold + 0.1){        	
        	calculateSimilarPairs(tempThreshold);
        }
		
        System.out.println("\nThe set of original object pairs whose similarity is above the threshold t(the real similar set)：" + simPairWithOptThreshold 
				+ "\nThe real similar set which have the same type：" + simPairWithOptThresholdAndSameType 
				+ "\nOptimal threshold：" + optThreshold
				+ "\nOptimal TP rate：" + optTPRate);
        
        while(true){        	
        	sc = new Scanner(System.in);
        	System.out.println();
        	System.out.println("Input the new instruction parameters："); 
        	newInstrParas =  sc.nextLine();
        	
        	int resultType = chooseType(newInstrParas);
        	System.out.println("The decision is type：" + resultType);
        }
	}
 	
	public int chooseType(String instrcution){
		
		int SWARVote = 0;
		int TLVote = 0;
		int UnchangedVote = 0;
		int[] newInstruction = new int[6];
		String[] paras = instrcution.split(",");
		for(int i = 0; i < paras.length; i++){
			newInstruction[i] = new Integer(paras[i]).intValue();
		}
		
		int[] similarDataIndex = findAllSimilarData(newInstruction);
		
		for(int i = 0; i < similarDataIndex.length; i++){
			if(similarDataIndex[i] == 1){
				System.out.println("Similar to the " + i + "-th data");
				if(dataMatrixWithTypeAndOptPerf[i][colunmNumberWithType - 1] == SWAR)
					SWARVote++;
				if(dataMatrixWithTypeAndOptPerf[i][colunmNumberWithType - 1] == TypeLegalization)
					TLVote++;
				if(dataMatrixWithTypeAndOptPerf[i][colunmNumberWithType - 1] == Unchanged)
					UnchangedVote++;
			}
		}
		
		System.out.println("SWARVote " + SWARVote);
		System.out.println("TLVote " + TLVote);
		System.out.println("UnchangedVote " + UnchangedVote);
		
		if((SWARVote > TLVote) && (SWARVote > UnchangedVote)){
			return SWAR;
		}else if(TLVote > UnchangedVote){
			return TypeLegalization;
		}else{
			return Unchanged;
		}
	}
	
	public int[] findAllSimilarData(int[] newInstruction){
		int[] similarDataIndex = new int[dataMatrix.length];
		for (int i = 0; i < dataMatrix.length; i++) {
        	int[] object = dataMatrix[i];
        	
        	for(int j = 0; j < object.length; j++){
        		System.out.print(object[j] + ",");
        	}
        	System.out.println();
        	
            double similarity = MinHash.similarityIndex(newInstruction, object);
            System.out.println("similarity with " + i + "-th data is: " + similarity);
            System.out.println();

            if(similarity >= optThreshold){
            	similarDataIndex[i] = 1;
            }else{
            	similarDataIndex[i] = 0;
            }
		}
		return similarDataIndex;
	}
	
	
	public void calculateSimilarPairs(Double threshold){
		
		int[] results = new int[6];
        for (int i = 0; i < dataMatrix.length; i++) {
        	int[] object1 = dataMatrix[i];
            int[] hash1 = hashes[i];
            int[] sigs1 = sigs[i];
            	
            for (int j = 0; j < i; j++) {
            	int[] object2 = dataMatrix[j];
                int[] hash2 = hashes[j];
                int[] sigs2 = sigs[j];
                
                // Compute the similarity between each pair of objects
                double similarity = MinHash.similarityIndex(object1, object2);
                // Compute the similarity between each pair of signatures
                double sigsSimilarity = MinHash.jaccardIndex(sigs1, sigs2);
                
                // Calculate the set of pairs which hash to the same bucket for ≥ 1 band
                for (int stage = 0; stage < bands; stage++) {
            		if (hash1[stage] == hash2[stage]) {
            			results[1]++;
            			break;
            		}
            	} 	
                	
                // Calculate the set of pairs whose signature similarity is above the threshold t
                if(sigsSimilarity >= threshold){                	
                	results[0]++;
                }
                
                /* Calculate the set which is the set of pairs whose signature similarity is above the threshold t
                 * AND hash to the same bucket for ≥ 1 band.
                 * In another words, the set that candidate pairs really do have similar signatures
                 */
                if(sigsSimilarity >= threshold){                	            
                	for (int stage = 0; stage < bands; stage++) {
                		if (hash1[stage] == hash2[stage]) {
                			results[2]++;
                			break;
                		}
                	}
                }

                // Calculate the set of original object pairs whose similarity is above the threshold t
                if(similarity >= threshold){                	
                	results[3]++;
//                    System.out.println("The" + i +"-th data is similar to the" + j + "-th data");
                	if(dataMatrixWithTypeAndOptPerf[i][colunmNumberWithType-1] == dataMatrixWithTypeAndOptPerf[j][colunmNumberWithType-1]){
                		results[5]++;
                	}
                }
                
                /* Calculate the set which is the set of pairs whose similarity of objects is above the threshold t
                 * AND whose signature similarity is likely above the threshold t
                 * AND hash to the same bucket for ≥ 1 band
                 * In another word, the set is the remaining candidate pairs actually represent similar sets, by removing dissimilar sets.
                 */
                if( (similarity >= threshold) && (sigsSimilarity >= threshold) ){
                	for (int stage = 0; stage < bands; stage++) {
                		if (hash1[stage] == hash2[stage]) {           				
                				results[4]++;
                			break;
                		}
                	}
                }       
                
            } 
        }
        
      Double TPRate = (double)((double)results[5]/(double)results[3]);
      System.out.println("\nThe set of original object pairs whose similarity is above the threshold：" + results[3] 
				+ "\nThe real similar set which have the same type：" + results[5] 
				+ "\nThreshold：" + threshold
				+ "\nTP rate：" + TPRate);
      if(TPRate > optTPRate){
      	simPairWithOptThreshold = results[3];
      	simPairWithOptThresholdAndSameType = results[5];
      	optTPRate = TPRate;
      	optThreshold = threshold;
      }
        
	}
	
	//Read the txt file
	public void readFile() throws Exception{ 
		BufferedInputStream fis = new BufferedInputStream(new FileInputStream(new File(filePath)));    
		reader = new BufferedReader(new InputStreamReader(fis,"utf-8"),bufferSize);
		String line = "";
		int rowNumber = 0;
		while((line = reader.readLine()) != null){	
			String[] elements = line.split(",");
			for(int colunmNumberWithType = 0; colunmNumberWithType < elements.length; colunmNumberWithType++){
				dataMatrixWithTypeContain3[rowNumber][colunmNumberWithType] = new Integer(elements[colunmNumberWithType]).intValue();
				if(rowNumber % 3 == 0){	
					dataMatrixWithTypeAndOptPerf[rowNumber/3][colunmNumberWithType] = dataMatrixWithTypeContain3[rowNumber][colunmNumberWithType];
					if(colunmNumberWithType != elements.length - 1){					
						dataMatrix[rowNumber/3][colunmNumberWithType] = (int)dataMatrixWithTypeAndOptPerf[rowNumber/3][colunmNumberWithType];
					}
				}
			}
			rowNumber++;
		}
		
		for(int i = 0; i < rowNumber; i++ ){
			for(int j = 0; j < colunmNumberWithType; j++){
				System.out.print(dataMatrixWithTypeContain3[i][j] + ",");
			}
			System.out.println();
		}
		
		System.out.println();
		for(int i = 0; i < rowNumber/3; i++ ){
			for(int j = 0; j < colunmNumberWithType + 1; j++){
				if(j == colunmNumberWithType)
					System.out.print(dataMatrixWithTypeAndOptPerf[i][j]);
				else
					System.out.print((int)dataMatrixWithTypeAndOptPerf[i][j] + ",");
			}
			System.out.println();
		}
		System.out.println();
		
		for(int i = 0; i < rowNumber/3; i++ ){
			for(int j = 0; j < colunmNumber; j++){	
				System.out.print(dataMatrix[i][j] + ",");
			}
			System.out.println();
		}
		System.out.println();
		
		fis.close();
		
		fis = new BufferedInputStream(new FileInputStream(new File(perfFilePath)));    
		reader = new BufferedReader(new InputStreamReader(fis,"utf-8"),bufferSize);
		line = "";
		rowNumber = 0;
		while((line = reader.readLine()) != null){	
			String[] elements = line.split(",");
			for(int colunmNumber = 0; colunmNumber < elements.length; colunmNumber++){
				perfData[rowNumber][colunmNumber] = new Double(elements[colunmNumber]).doubleValue();
			}
			rowNumber++;
		}
		
		for(int i = 0; i < rowNumber; i++ ){
			for(int j = 0; j < perfSize; j++){
				System.out.print(perfData[i][j] + ",");
			}
			System.out.println();
		}
		
		fis.close();
	}
	
	public void calculateWithCertainWeight(Double weightOfA, Double weightOfB, Double weightOfC) throws Exception{
		int currentRow = 0;
		Double type1Perf, type2Perf, type3Perf, optPerf;
		int type1, type2, type3, optType;
		for(int i = 0; i < rowNumber/3; i++){
			type1Perf = weightOfA * perfData[currentRow][0] + weightOfB * perfData[currentRow][1] + weightOfC * perfData[currentRow][2];
			type1 = dataMatrixWithTypeContain3[currentRow][colunmNumberWithType - 1];
			currentRow++;
			type2Perf = weightOfA * perfData[currentRow][0] + weightOfB * perfData[currentRow][1] + weightOfC * perfData[currentRow][2];
			type2 = dataMatrixWithTypeContain3[currentRow][colunmNumberWithType - 1];
			currentRow++;
			type3Perf = weightOfA * perfData[currentRow][0] + weightOfB * perfData[currentRow][1] + weightOfC * perfData[currentRow][2];
			type3 = dataMatrixWithTypeContain3[currentRow][colunmNumberWithType - 1];
			currentRow++;
			
			if((type1Perf < type2Perf) && (type1Perf < type3Perf)){
				optPerf = type1Perf;
				optType = type1;
			}else if(type2Perf < type3Perf){
				optPerf = type2Perf;
				optType = type2;
			}else{
				optPerf = type3Perf;
				optType = type3;
			}
			
			dataMatrixWithTypeAndOptPerf[i][colunmNumberWithType - 1] = optType;
			dataMatrixWithTypeAndOptPerf[i][colunmNumberWithType] = optPerf;
		}
		
		System.out.println();
		for(int i = 0; i < rowNumber/3; i++ ){
			for(int j = 0; j < colunmNumberWithType + 1; j++){
				if(j == colunmNumberWithType)
					System.out.print(dataMatrixWithTypeAndOptPerf[i][j]);
				else
					System.out.print((int)dataMatrixWithTypeAndOptPerf[i][j] + ",");
			}
			System.out.println();
		}
		System.out.println();
	}

}