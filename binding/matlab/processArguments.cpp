

void checkNombreInputParametres(int nrhs, unsigned int min, unsigned int max, std::string message = ""){
	if (nrhs < min){
		mexErrMsgIdAndTxt("MATLAB:yprime:invalidNumInputs",
			message.c_str());
	}
	else if (nrhs > max){
		mexErrMsgIdAndTxt("MATLAB:yprime:invalidNumInputs",
			message.c_str());
	}
	

}

std::vector<std::vector<Eigen::Vector3d> > getParameterAllMarkers(const mxArray*prhs[], unsigned int idx, unsigned int nMark=-1){
    /* Check data type of input argument */
    if (!(mxIsDouble(prhs[idx]))) {
        std::ostringstream msg;
        msg << "Argument " << idx+1 << " must be of type double.";
        mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",msg.str().c_str());
    }

    mwSize m=mxGetM(prhs[idx]); // XYZ
    mwSize n=mxGetN(prhs[idx]); // Nombre de markers

    /* Get the number of elements in the input argument */
    if (m<3 || m>4){
        std::ostringstream msg;
        msg << "Wrong size! Input markers matrix should be 3 or 4 lines";
        mexErrMsgTxt(msg.str().c_str());
    }

    /* Get number of dimension */
    mwSize nsubs=mxGetNumberOfDimensions(prhs[idx]);
    if (nsubs < 2 || nsubs > 3){
        std::ostringstream msg;
        msg << "Wrong size! Input markers matrix should be a 3-dimensions or 2-dimensions matrix";
        mexErrMsgTxt(msg.str().c_str());
    }
    /* Get number of timeframes */
    double nFrames(1);
    if (nMark==-1)
        nMark = (mxGetDimensions(prhs[idx]))[1];
    if (nsubs == 3)
        nFrames = (mxGetDimensions(prhs[idx]))[2];

    /* Get the number of elements in the input argument */
    if (n/nFrames!=nMark){ // puisque les dimensions supplémentaires sont ajoutées a la fin, il faut / par nFrames
        std::ostringstream msg;
        msg << "Wrong size! Input markers matrix should should have the same number of technical markers (" << nMark << " markers.";
        mexErrMsgTxt(msg.str().c_str());
    }

    /* Get a pointer to the values */
    double *markers = mxGetPr(prhs[idx]);



    /* Créer la sortie */
    std::vector<std::vector<Eigen::Vector3d> > markersOverTime;

    /* Stocker les valeurs dans le format de sortie */
    unsigned int cmp(0);
    for (unsigned int i=0; i<nFrames; ++i){
        std::vector<Eigen::Vector3d> markers_tp; // Markers a un temps i

        for (unsigned int j=0; j<nMark; ++j){
            Eigen::Vector3d tp(Eigen::Vector3d(markers[m*cmp+0], // m est 3 ou 4
                                           markers[m*cmp+1],
                                           markers[m*cmp+2]));
            markers_tp.push_back(tp);
            ++cmp;
        }
        markersOverTime.push_back(markers_tp);
    }

    /* Retourner la matrice */
    return markersOverTime;
}
std::vector<std::vector<s2mAttitude> > getParameterAllIMUs(const mxArray*prhs[], unsigned int idx, unsigned int nMark=-1){
    /* Check data type of input argument */
    if (!(mxIsDouble(prhs[idx]))) {
        std::ostringstream msg;
        msg << "Argument " << idx+1 << " must be of type double.";
        mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",msg.str().c_str());
    }

    /* Get number of dimension */
    mwSize nsubs=mxGetNumberOfDimensions(prhs[idx]);
    mwSize m=(mxGetDimensions(prhs[idx]))[0]; // nligne
    mwSize n=(mxGetDimensions(prhs[idx]))[1]; // ncol

    /* Get the number of elements in the input argument */
    if (m!=n){
        std::ostringstream msg;
        msg << "Wrong size! IMUs must be a 3x3 or 4x4 matrix";
        mexErrMsgTxt(msg.str().c_str());
    }
    if (m<3 || m>4){
        std::ostringstream msg;
        msg << "Wrong size! IMUs must be a 3x3 or 4x4 matrix";
        mexErrMsgTxt(msg.str().c_str());
    }


    if (nsubs < 2 || nsubs > 4){
        std::ostringstream msg;
        msg << "Wrong size! Input IMUs matrix should be a 3-dimensions or 2-dimensions matrix";
        mexErrMsgTxt(msg.str().c_str());
    }
    /* Get number of IMUs */
    mwSize nIMUs(1);
    if (nsubs >= 3)
        nIMUs = (mxGetDimensions(prhs[idx]))[2];

    /* Get number of timeframes */
    double nFrames(1);
    if (nsubs >= 4)
        nFrames = (mxGetDimensions(prhs[idx]))[3];

    /* Get a pointer to the values */
    double *imus = mxGetPr(prhs[idx]);

    /* Créer la sortie */
    std::vector<std::vector<s2mAttitude> > imuOverTime;

    /* Stocker les valeurs dans le format de sortie */
    unsigned int cmp(0);
    for (unsigned int i=0; i<nFrames; ++i){
        std::vector<s2mAttitude> imus_tp; // IMUs a un temps i
        for (unsigned int j=0; j<nIMUs; ++j){
            Eigen::Matrix3d rot;
            Eigen::Vector3d trans;
            if (n==4){
                rot <<  imus[i*16*nIMUs+j*16+0], imus[i*16*nIMUs+j*16+4], imus[i*16*nIMUs+j*16+8],
                        imus[i*16*nIMUs+j*16+1], imus[i*16*nIMUs+j*16+5], imus[i*16*nIMUs+j*16+9],
                        imus[i*16*nIMUs+j*16+2], imus[i*16*nIMUs+j*16+6], imus[i*16*nIMUs+j*16+10];
                trans << imus[i*16*nIMUs+j*16+12], imus[i*16*nIMUs+j*16+13], imus[i*16*nIMUs+j*16+14];
            } else {
                rot <<  imus[i*9*nIMUs+j*9+0], imus[i*9*nIMUs+j*9+3], imus[i*9*nIMUs+j*9+6],
                        imus[i*9*nIMUs+j*9+1], imus[i*9*nIMUs+j*9+4], imus[i*9*nIMUs+j*9+7],
                        imus[i*9*nIMUs+j*9+2], imus[i*9*nIMUs+j*9+5], imus[i*9*nIMUs+j*9+8];
                trans.setZero();
            }
            imus_tp.push_back(s2mAttitude(rot, trans));
        }
        imuOverTime.push_back(imus_tp);
    }

    /* Retourner la matrice */
    return imuOverTime;
}


s2mGenCoord getVector(const mxArray*prhs[], unsigned int idx, std::string type = ""){
    /* Check data type of input argument */
    if (!(mxIsDouble(prhs[idx]))) {
        std::ostringstream msg;
        msg << "Argument " << idx+1 << " must be of type double.";
        mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",msg.str().c_str());
    }

    mwSize m=mxGetM(prhs[idx]); // line
    mwSize n=mxGetN(prhs[idx]); // line

    int length(0);
    if (m!=1 && n==1) // on permet le nombre juste ou juste une valeur qui sera dupliquée
        length = m;
    else if (n!=1 && m == 1)
        length = n;
    else {
        std::ostringstream msg;
        msg << type << " should be a vector";
        mexErrMsgTxt(msg.str().c_str());
    }

    double *q=mxGetPr(prhs[idx]); //matrice de position

    // Coordonnées généralisées du modèle envoyées vers lisible par le modèle
    s2mGenCoord vect(Eigen::VectorXd::Zero (length));
    for (unsigned int i=0; i<length; i++)
        vect(i) = q[i];

    return vect;
}

s2mGenCoord getVector(const mxArray*prhs[], unsigned int idx, unsigned int length, std::string type = ""){
    mwSize m=mxGetM(prhs[idx]); // line
    /* Get the number of elements in the input argument */
    if ( !(m==length || m==1) ){ // on permet le nombre juste ou juste une valeur qui sera dupliquée
        std::ostringstream msg;
        msg << "Wrong size! (Input " << type << "), should be " << length;
        mexErrMsgTxt(msg.str().c_str());
    }

    return getVector(prhs, idx, type);
}
Eigen::Vector3d getVector3d(const mxArray*prhs[], unsigned int idx){
    return getVector(prhs, idx, 3, "Vector3d").vector();
}

std::vector<s2mGenCoord> getParameterQ(const mxArray*prhs[], unsigned int idx, unsigned int nDof, std::string type = "q"){

		/* Check data type of input argument */
        if (!(mxIsDouble(prhs[idx]))) {
			std::ostringstream msg;
			msg << "Argument " << idx+1 << " must be of type double.";
            mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",msg.str().c_str());
        }
		
        mwSize m=mxGetM(prhs[idx]); // line
        /* Get the number of elements in the input argument */
        if (m!=nDof){
			std::ostringstream msg;
			msg << "Wrong size! (Input " << type << "), should be " << nDof;
            mexErrMsgTxt(msg.str().c_str());
        }

        /* Get the number of frames in the input argument */
        mwSize nFrames=mxGetN(prhs[idx]); // line

        double *q=mxGetPr(prhs[idx]); //matrice de position
        
        // Coordonnées généralisées du modèle envoyées vers lisible par le modèle
        std::vector<s2mGenCoord> Q;
        for (unsigned int j=0; j<nFrames; ++j){
            s2mGenCoord Q_tp(Eigen::VectorXd::Zero(nDof));
            for (unsigned int i=0; i<nDof; i++)
                Q_tp[i] = q[j*nDof+i];

            Q.push_back(Q_tp);
        }
		return Q;
}
std::vector<s2mGenCoord> getParameterQddot(const mxArray*prhs[], unsigned int idx, unsigned int nDof){
	return getParameterQ(prhs, idx, nDof, "qddot");
}
std::vector<s2mGenCoord> getParameterQdot(const mxArray*prhs[], unsigned int idx, unsigned int nDof){
	return getParameterQ(prhs, idx, nDof, "qdot");
}
std::vector<s2mTau> getParameterTau(const mxArray*prhs[], unsigned int idx, unsigned int nControl, unsigned int nRoot){
    std::vector<s2mGenCoord> AllTau_tp = getParameterQ(prhs, idx, nControl, "tau");
    std::vector<s2mTau> AllTau;

    for (unsigned int j=0; j<AllTau_tp.size(); ++j){
        s2mTau Tau_tp(Eigen::VectorXd::Zero(nControl+nRoot));

        for (unsigned int i=0; i<nRoot; ++i) // Root segment
            Tau_tp(i) =  0;
        for (unsigned int i=nRoot; i < nControl+nRoot; ++i) // Everything else
            Tau_tp(i) = (*(AllTau_tp.begin()+j))[i-nRoot];

        AllTau.push_back(Tau_tp);
    }
    return AllTau;
}

std::vector<std::vector<RigidBodyDynamics::Math::SpatialVector> > getForcePlate(const mxArray*prhs[], unsigned int idx){
	if (!(mxIsDouble(prhs[idx]))) {
		mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",
			   "Argument 6 must be of type double.");
	}

    const mwSize* dims (mxGetDimensions(prhs[idx]));
    mwSize mPF(dims[0]); // Nombre de lignes (devrait etre 6)
    mwSize nPF(dims[1]); // Nombre de plateformes
    mwSize timeStamp;

    if (mPF*nPF == mxGetNumberOfElements(prhs[idx]) ) // s'il n'y a pas de dimension temps
        timeStamp = 1;
    else
        timeStamp = dims[2]; // Nombre de temps


	if (mPF!=6){ // must be 6 lines (mx, my, mz, fx, fy, fz)
        std::string errorMessage = "Wrong size! (Input forceplates), should be 6xNb_Forceplates x time";
		mexErrMsgTxt(errorMessage.c_str());
	}
	double *pf = mxGetPr(prhs[idx]); // Matrice des plateforme de force

	// stockage des plateformes
    std::vector<std::vector<RigidBodyDynamics::Math::SpatialVector> > PF;
    unsigned int cmp(0);
    for (unsigned int j=0; j<timeStamp; ++j){
        std::vector<RigidBodyDynamics::Math::SpatialVector> PF_tp;
        for (unsigned int i=0; i<nPF; ++i){ // pour chaque plateforme
            RigidBodyDynamics::Math::SpatialVector tp(pf[0+cmp*6], pf[1+cmp*6], pf[2+cmp*6], pf[3+cmp*6], pf[4+cmp*6], pf[5+cmp*6]);
            PF_tp.push_back(tp);
            cmp++;
        }
        PF.push_back(PF_tp);
    }
	
	return PF;
}
double getDouble(const mxArray*prhs[], unsigned int idx, std::string type = "d"){

        /* Check data type of input argument */
        if (!(mxIsDouble(prhs[idx]))) {
            std::ostringstream msg;
            msg << "Argument " << idx+1 << " must be of type double.";
            mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",msg.str().c_str());
        }

        mwSize m=mxGetM(prhs[idx]); // line
        /* Get the number of elements in the input argument */
        if (m!=1){
            std::ostringstream msg;
            msg << "Wrong size! (Input " << type << "), should be 1";
            mexErrMsgTxt(msg.str().c_str());
        }
        double *q=mxGetPr(prhs[idx]); //matrice de position
        return q[0];
}
int getInteger(const mxArray*prhs[], unsigned int idx, std::string type = "i"){
    return getDouble(prhs, idx, type);
}

s2mString getString(const mxArray*prhs[], unsigned int idx){
    /* Check data type of input argument */
    if (!(mxIsChar(prhs[idx]))) {
        std::ostringstream msg;
        msg << "Argument " << idx+1 << " must be of type char.";
        mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",msg.str().c_str());
    }

    return mxArrayToString(prhs[idx]);

}

bool getBool(const mxArray*prhs[], unsigned int idx){
    bool out;

    if (mxIsDouble(prhs[idx]))
        out = *(mxGetPr(prhs[idx]));
    else if (mxIsLogical(prhs[idx]))
        out = *(mxGetLogicals(prhs[idx]));
    else{
        std::ostringstream msg;
        msg << "Argument " << idx+1 << " must be of type bool.";
        mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",msg.str().c_str());
    }
    return out;

}


bool isStateExist(const mxArray*prhs[], unsigned int nMus, int idx, bool &isThere){
    if (idx != -1){
        if (!(mxIsDouble(prhs[idx]))) {
            std::ostringstream msg;
            msg << "Argument " << idx+1 << " must be of type double.";
            mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",msg.str().c_str());
        }

        mwSize m=mxGetM(prhs[idx]); // line
        /* Get the number of elements in the input argument */
        if (m!=nMus){
            std::ostringstream msg;
            msg << "Wrong size! (Input state muscle parameter), should be " << nMus;
            mexErrMsgTxt(msg.str().c_str());
        }

        isThere = true;


    }
    else
        isThere = false;

    return isThere;
}

std::vector<std::vector<s2mMuscleStateActual> > getParameterMuscleState(const mxArray*prhs[], int idxExcitation, int idxActivation, unsigned int nMus){


    /* Get the number of frames in the input argument */
    // Regarde ce qui se passe pour l'excitation
    bool isThereExcitation;
    mwSize nFramesE(0);
    double *stateExcitation;
    if (isStateExist(prhs, nMus, idxExcitation, isThereExcitation)){
        stateExcitation=mxGetPr(prhs[idxExcitation]); //matrice de position
        nFramesE = mxGetN(prhs[idxExcitation]); // line
    }

    // Regarde ce qui se passe pour l'activation
    bool isThereActivation;
    mwSize nFramesA(0);
    double *stateActivation; //matrice de position
    if (isStateExist(prhs, nMus, idxActivation, isThereActivation)){
        stateActivation=mxGetPr(prhs[idxActivation]); //matrice de position
        nFramesA = mxGetN(prhs[idxActivation]); // line
    }

    // S'assurer que l'excitation et l'activation ont la même taille
    if (isThereActivation && isThereExcitation && nFramesA != nFramesE) { // Si on se soucis de l'excitation
        std::ostringstream msg;
        msg << "Wrong size! Input excitation and activation should be the same";
        mexErrMsgTxt(msg.str().c_str());
    }
    mwSize nFramesTotal;
    if (isThereActivation)
        nFramesTotal = nFramesA;
    else if (isThereExcitation)
        nFramesTotal = nFramesE;
    else{
        std::ostringstream msg;
        msg << "At least excitation or activation should be send";
        mexErrMsgTxt(msg.str().c_str());
    }


    // Coordonnées généralisées du modèle envoyées vers lisible par le modèle
    std::vector<std::vector<s2mMuscleStateActual> > States;
    for (unsigned int j=0; j<nFramesTotal; ++j){
        std::vector<s2mMuscleStateActual> States_tp;
        for (unsigned int i=0; i<nMus; i++)
            if (isThereExcitation && isThereActivation){
                States_tp.push_back( s2mMuscleStateActual(stateExcitation[j*nMus+i], stateActivation[j*nMus+i]));
            }
            else if (!isThereExcitation && isThereActivation){
                States_tp.push_back( s2mMuscleStateActual(0, stateActivation[j*nMus+i]));
            }
            else if (isThereExcitation && !isThereActivation){
                States_tp.push_back( s2mMuscleStateActual(stateExcitation[j*nMus+i], 0));
            }

        States.push_back(States_tp);
    }
    return States;
}

std::vector<std::vector<s2mMuscleStateActual> > getParameterMuscleStateActivation(const mxArray*prhs[], int idxActivation, unsigned int nMus){
    return getParameterMuscleState(prhs, -1, idxActivation, nMus);
}
std::vector<std::vector<s2mMuscleStateActual> > getParameterMuscleStateExcitation(const mxArray*prhs[], int idxExcitation, unsigned int nMus){
    return getParameterMuscleState(prhs, idxExcitation, -1, nMus);
}
std::vector<Eigen::VectorXd> getParameterMuscleForceNorm(const mxArray*prhs[], unsigned int idx, unsigned int nMus, std::string type = "muscle force"){

        /* Check data type of input argument */
        if (!(mxIsDouble(prhs[idx]))) {
            std::ostringstream msg;
            msg << "Argument " << idx+1 << " must be of type double.";
            mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",msg.str().c_str());
        }

        mwSize m=mxGetM(prhs[idx]); // line
        /* Get the number of elements in the input argument */
        if (m!=nMus){
            std::ostringstream msg;
            msg << "Wrong size! (Input " << type << "), should be " << nMus;
            mexErrMsgTxt(msg.str().c_str());
        }

        /* Get the number of frames in the input argument */
        mwSize nFrames=mxGetN(prhs[idx]); // line

        double *f=mxGetPr(prhs[idx]); //matrice de position

        // Forces musculaires
        std::vector<Eigen::VectorXd> F;
        for (unsigned int j=0; j<nFrames; ++j){
            Eigen::VectorXd F_tp = Eigen::VectorXd::Zero (nMus);
            for (unsigned int i=0; i<nMus; i++)
                F_tp[i] = f[j*nMus+i];

            F.push_back(F_tp);
        }
        return F;
}


std::vector<std::vector<s2mNodeMuscle> > getMusclePosition(const mxArray*prhs[], unsigned int idx, Eigen::VectorXd nPointsByMuscles){
    /* Check data type of input argument */
    if (!(mxIsDouble(prhs[idx]))) {
        std::ostringstream msg;
        msg << "Argument " << idx+1 << " must be of type double.";
        mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",msg.str().c_str());
    }

    mwSize nRows=mxGetM(prhs[idx]); // line
    if (nRows<3 || nRows>4){
        std::ostringstream msg;
        msg << "Muscle position should be a column-wise matrix with 3 or 4 rows (where the 4th is a line of 1)";
        mexErrMsgTxt(msg.str().c_str());
    }

    /* Get the number of muscles in the input argument */
    int nVia(0);
    for (int i = 0; i<nPointsByMuscles.size(); ++i){
        nVia += nPointsByMuscles(i);
    }

    mwSize nCol=mxGetN(prhs[idx]); // col
    if (nCol!=nVia){ // on permet le nombre juste ou juste une valeur qui sera dupliquée
        std::ostringstream msg;
        msg << "muscle position should be a column-wise matrix with columns = " << nVia;
        mexErrMsgTxt(msg.str().c_str());
    }

    // Recueillir les données
    double *via=mxGetPr(prhs[idx]); //matrice de position

    // Préparer la matrice de sortie
    std::vector<std::vector<s2mNodeMuscle> > out;
    int cmpMus(0);
    for (unsigned int i=0; i<nPointsByMuscles.rows(); ++i){
        // Préparer les matrices intermédiaires (chaque muscle)
        std::vector<s2mNodeMuscle> mus;
        for (unsigned int j=0; j<nPointsByMuscles(i); ++j){
            mus.push_back(s2mNodeMuscle(Eigen::Vector3d(via[cmpMus*nRows+0],via[cmpMus*nRows+1],via[cmpMus*nRows+2])));
            ++cmpMus;
        }
        out.push_back(mus);
    }

    // Retourner la structure remplie
    return out;
}

std::vector<Eigen::MatrixXd> getMusclePointsJaco(const mxArray*prhs[], unsigned int idx, Eigen::VectorXd nPointsByMuscles, int nQ){
    /* Check data type of input argument */
    if (!(mxIsDouble(prhs[idx]))) {
        std::ostringstream msg;
        msg << "Argument " << idx+1 << " must be of type double.";
        mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",msg.str().c_str());
    }

    mwSize nRows=mxGetM(prhs[idx]); // line

    int nVia(0);
    for (int i = 0; i<nPointsByMuscles.size(); ++i){
        nVia += nPointsByMuscles(i);
    }
    if (nRows!=nVia*3){
        std::ostringstream msg;
        msg << "Jacobian must be a matrix with 3*nMus*nVia/nMus X nQ (" << nVia*3 << "," << nQ << ")";
        mexErrMsgTxt(msg.str().c_str());
    }

    mwSize nCol=mxGetN(prhs[idx]); // col
    if (nCol!=nQ){
        std::ostringstream msg;
        msg << "Jacobian must be a matrix with 3*nMus X nQ";
        mexErrMsgTxt(msg.str().c_str());
    }

    // Recueillir les données
    double *jaco=mxGetPr(prhs[idx]); //matrice de position

    // Préparer la matrice de sortie
    std::vector<Eigen::MatrixXd> jacoOut;
    int cmpMus(0);

    for (unsigned int i=0; i<nPointsByMuscles.rows(); ++i){
        // Préparer les matrices intermédiaires (chaque muscle)
        Eigen::MatrixXd mus((int)nPointsByMuscles(i)*3, nQ); // Théoriquement, c'est toujours un int
        for (unsigned int j=0; j<nPointsByMuscles(i); ++j){
            // Stocker
            for (int k1 = 0; k1<3; ++k1)
                for (int k2 = 0; k2 <nQ; ++k2)
                    mus(3*j+k1, k2) = jaco[3*cmpMus+k1+nVia*3*k2];
            // Incrémenter l'index de muscle
            ++cmpMus;
        }

        jacoOut.push_back(mus);
    }

    // Retourner la structure remplie
    return jacoOut;

}
