#include <arrayfire.h>
#include <vector>
#include <cstdint>
#include <cmath>
#include <random>
#include "Spk_Delay_MNGR.h"
#include "SynMatrices.h"

UDFPlasticity::UDFPlasticity(const SynMatrices &_synHost)
	: synHost(_synHost), scaleFactor(DEF_SCALE_FAC)
{
	//uint32_t range = host.dlyRange;

	//uint32_t sSz = host.src.size;
	uint32_t tSz = host.tar.size;
	uint32_t** dlyMat = hostdlyMat;

	//std::vector<std::vector<uint32_t>> srcs(maxDly-minDly, std::vector<uint32_t>);
	//std::vector<std::vector<uint32_t>> tars(maxDly-minDly, std::vector<uint32_t>);
	//for (uint32_t i = 0; i < sSz; i++) {
	//	for (uint32_t j = 0; j < tSz; j++) {
	//		if (dlyMat[i][j] !=0) {
	//			srcs[dlyMat[i][j] - minDly].push_back(i);
	//			tars[dlyMat[i][j] - minDly].push_back(j);
	//		}
	//	}
	//}

	float U_mean;
	float U_std;
	float D_mean;
	float D_std;
	float F_mean;
	float F_std;

	if (host.src.polarity == 1) {
		if (host.tar.polarity == 1) { 	// EE
			U_mean = EE_U_MN;
			U_std = EE_U_SD;
			D_mean = EE_D_MN;
			D_std = EE_D_SD;
			F_mean = EE_F_MN;
			F_std = EE_F_SD;
		} else { 						// EI
			U_mean = EI_U_MN;
			U_std = EI_U_SD;
			D_mean = EI_D_MN;
			D_std = EI_D_SD;
			F_mean = EI_F_MN;
			F_std = EI_F_SD;
		}
	} else {
		if (host.tar.polarity == 1) { 	// IE
			U_mean = IE_U_MN;
			U_std = IE_U_SD;
			D_mean = IE_D_MN;
			D_std = IE_D_SD;
			F_mean = IE_F_MN;
			F_std = IE_F_SD;
		} else { 						// II
			U_mean = II_U_MN;
			U_std = II_U_SD;
			D_mean = II_D_MN;
			D_std = II_D_SD;
			F_mean = II_F_MN;
			F_std = II_F_SD;
		}
	}

	for (uint32_t i = 0; i < synHost.tarHost.size; i++)
	{
		uint32_t indeg = (uint32_t) synHost.indices[i].dims(0);
		UDFuR.push_back(constant(0, dim4(5, indeg), f32));
		UDFuR[i].row(0) = abs((randn(dim4(1, indeg), f32)*U_std)+U_mean);
		UDFuR[i].row(1) = abs((randn(dim4(1, indeg), f32)*D_std)+D_mean);
		UDFuR[i].row(2) = abs((randn(dim4(1, indeg), f32)*F_std)+F_mean);
		//UDFuR[i](3, span) = 0;
		UDFuR[i](4, span) = 1;
	}



// TODO: actually fill the right data structure... duh...

//	for (uint32_t d = 0; d < (maxDly-minDly); d++) {
//		uint32_t szLoc = srcs[d].size();
//		U.push_back(
//			sparse( sSz, tSz, szLoc,
//			af::abs((randn(szLoc, f32)*U_std)+U_mean),
//			array(szLoc, srcs[d].data()),
//			array(szLoc, tars[d].data()),
//			f32, AF_STORAGE_COO )	); 
//		D.push_back(
//			sparse( sSz, tSz, szLoc,
//			af::abs((randn(szLoc, f32)*D_std)+D_mean),
//			array(szLoc, srcs[d].data()),
//			array(szLoc, tars[d].data()),
//			f32, AF_STORAGE_COO )	); 
//		F.push_back(
//			sparse( sSz, tSz, szLoc,
//			af::abs((randn(szLoc, f32)*F_std)+F_mean),
//			array(szLoc, srcs[d].data()),
//			array(szLoc, tars[d].data()),
//			f32, AF_STORAGE_COO )	);  
//		u.push_back(
//			sparse( sSz, tSz, szLoc,
//			constant(0, szLoc, f32),
//			array(szLoc, srcs[d].data()),
//			array(szLoc, tars[d].data()),
//			f32, AF_STORAGE_COO )	); 
//		R.push_back(
//			sparse( sSz, tSz, szLoc,
//			constant(1, szLoc, f32),
//			array(szLoc, srcs[d].data()),
//			array(szLoc, tars[d].data()),
//			f32, AF_STORAGE_COO )	); 
//	}
}

array UDFPlasticity::perform(	const array &udfur,
								const array &tdiff,
								const array &wts	)
{
	udfur.row(3) = udfur.row(0) + 
		(udfur.row(3) * (1 - udfur.row(0)) * af::exp(-tdiff/udfur.row(2)));
	udfur.row(4) = 1 + ((udfur.row(4) - (udfur.row(3) * udfur.row(3)) - 1)
		* af::exp(-tdiff/udfur.row(1)));
	return scaleFactor * wts * udfur.row(3) * udfur.row(4);
}

float* UDFPlasticity::generateUDFVals(	const GenericNeuron::Polarity srcPol,
										const GenericNeuron::Polarity tarPol	)
{
	float* udfs = new float[5];
	if (srcPol == 1) {
		if (tarPol == 1) { 	// EE
			U_mean = EE_U_MN;
			U_std = EE_U_SD;
			D_mean = EE_D_MN;
			D_std = EE_D_SD;
			F_mean = EE_F_MN;
			F_std = EE_F_SD;
		} else { 						// EI
			U_mean = EI_U_MN;
			U_std = EI_U_SD;
			D_mean = EI_D_MN;
			D_std = EI_D_SD;
			F_mean = EI_F_MN;
			F_std = EI_F_SD;
		}
	} else {
		if (tarPol == 1) { 	// IE
			U_mean = IE_U_MN;
			U_std = IE_U_SD;
			D_mean = IE_D_MN;
			D_std = IE_D_SD;
			F_mean = IE_F_MN;
			F_std = IE_F_SD;
		} else { 						// II
			U_mean = II_U_MN;
			U_std = II_U_SD;
			D_mean = II_D_MN;
			D_std = II_D_SD;
			F_mean = II_F_MN;
			F_std = II_F_SD;
		}
	}
	udfs[0] = std::abs(U_mean + (normDist(gen)*U_std));
	udfs[1] = std::abs(D_mean + (normDist(gen)*D_std));
	udfs[2] = std::abs(F_mean + (normDist(gen)*F_std));
	udfs[3] = 0;
	udfs[0] = 1;

}