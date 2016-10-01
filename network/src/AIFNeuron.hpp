#include <arrayfire.h>
#include <cstdint>
#include <vector>
#include "Utils.h"


#ifndef AIFNEURON_H_
#define AIFNEURON_H_

#define DEF_V_rest -70f //mV
#define DEF_V_reset -55f //mV
#define INIT_THRESH -50f //mV
#define DEF_E_DEC 3f //ms
#define DEF_I_DEC 6f //ms
#define E_CM_MAX 28f
#define E_CM_MIN 24f
#define I_CM_MAX 26f
#define I_CM_MIN 18f
#define DEF_E_REF 3f
#define DEF_I_REF 6f
#define DEF_TAU_W 144f
#define DEF_I_BG 18f
#define DEF_CM_EXC 30f
#define DEF_CM_INH 20f
#define DEF_NSD 0.1f


class AIFNeuron {

	public:

		af::array* V_mem;
		af::array* V_buff;
		af::array* w;
		af::array* w_buff;
		af::array* thresholds;

		af::array* lastSpkTime;
		af::array* lst_buff;
		af::array* adpt;

		af::array online;
		af::array spks; // No need to buffer; copied by SynMat
		af::array spkHistory;
		af::array* I_e;
		af::array* I_i;
		af::array* I_bg;
		af::array* S_e;
		af::array* S_i;
		af::array Cm;
		af::array eFlip;
		af::array iFlip;

		std::vector<SynMatrices> incoExcSyns;
		std::vector<SynMatrices> incoInhSyns;

		uint32_t* exInDegs; //useful to keep these for pruner
		uint32_t* inInDegs;
		uint32_t* outDegs;

		float iDecay;
		float eDecay;
		float V_rest;
		float V_reset;
		float tauW;
		float noiseSD;
		//uint32_t network_id;
		uint32_t refP;

		float* x;
		float* y;
		float* z;

		uint32_t size;
		uint8_t polarity;

		//TODO: give option to lay out in lattice: more efficient
		AIFNeuron(	const Network net,
					const uint32_t size, 
					const uint8_t _polarity,
				 	const Position minPos,
				 	const Position maxPos );
		~AIFNeuron();

		void runForward(const uint32_t t, const float dt);
		void pushBuffers();
		Position* getPositions();
		Position getPosition(uint32_t index);

};

#endif // AIFNEURON_H_