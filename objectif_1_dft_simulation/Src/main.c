// programme pour calculer  la DFT en simulation sur un tableau de données imposés
int module(short * tabSig, int k);

extern short TabSig ;

int k = 0;
int tab[64];



int main(void){
	for (k=0 ; k<64; k++){
			tab[k]=module(&TabSig,k);
	}

	while (1) {}
}
