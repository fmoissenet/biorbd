
void S2M_ContactsPosition( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray*prhs[] ){
	
	// Verifier les arguments d'entrée
	checkNombreInputParametres(nrhs, 3, 3, "3 arguments are required where the 2nd is the handler on the model and 3rd is the Q");
	// Recevoir le model
	s2mMusculoSkeletalModel * model = convertMat2Ptr<s2mMusculoSkeletalModel>(prhs[1]);
    unsigned int nQ = model->nbQ(); /* Get the number of DoF */
					  
	// Recevoir Q
    s2mGenCoord Q = *getParameterQ(prhs, 2, nQ).begin();
	
	// Trouver où sont les marqueurs
    std::vector<Eigen::Vector3d> Contact_tp = model->constraintsInGlobal(*model,Q);
        		  
					  
	/* Create a matrix for the return argument */ 
    plhs[0] = mxCreateDoubleMatrix( 3, Contact_tp.size(), mxREAL);
    double *contact = mxGetPr(plhs[0]);
	
	// Remplir le output
    std::vector<Eigen::Vector3d>::iterator it=Contact_tp.begin();
    for (unsigned int i=0; (it+i)!=Contact_tp.end(); ++i){
        contact[i*3] = (*(it+i))(0);
        contact[i*3+1] = (*(it+i))(1);
        contact[i*3+2] = (*(it+i))(2);
	}
		
	return;
}
        
