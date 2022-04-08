#include "jvm.h"
#include "leitor.h"
#include "interpretador.h"

JVM *jvm = NULL;
instrucao* instrucoes;
void (*func_ptr[202])(frame*,u1,u1) = {nop_impl,aconst_null_impl,iconst_m1_impl,iconst_0_impl,iconst_1_impl,iconst_2_impl,iconst_3_impl,iconst_4_impl,iconst_5_impl,lconst_0_impl,lconst_1_impl,fconst_0_impl,fconst_1_impl,fconst_2_impl,dconst_0_impl,dconst_1_impl,bipush_impl,sipush_impl,ldc_impl,ldc_w_impl,ldc2_w_impl,iload_impl,lload_impl,fload_impl,dload_impl,aload_impl,iload_0_impl,iload_1_impl,iload_2_impl,iload_3_impl,lload_0_impl,lload_1_impl,lload_2_impl,lload_3_impl,fload_0_impl,fload_1_impl,fload_2_impl,fload_3_impl,dload_0_impl,dload_1_impl,dload_2_impl,dload_3_impl,aload_0_impl,aload_1_impl,aload_2_impl,aload_3_impl,iaload_impl,laload_impl,faload_impl,daload_impl,aaload_impl,baload_impl,caload_impl,saload_impl,istore_impl,lstore_impl,fstore_impl,dstore_impl,astore_impl,istore_0_impl,istore_1_impl,istore_2_impl,istore_3_impl,lstore_0_impl,lstore_1_impl,lstore_2_impl,lstore_3_impl,fstore_0_impl,fstore_1_impl,fstore_2_impl,fstore_3_impl,dstore_0_impl,dstore_1_impl,dstore_2_impl,dstore_3_impl,astore_0_impl,astore_1_impl,astore_2_impl,astore_3_impl,iastore_impl,lastore_impl,fastore_impl,dastore_impl,aastore_impl,bastore_impl,castore_impl,sastore_impl,pop_fantasma,pop2_fantasma,dup_impl,dup_x1_impl,dup_x2_impl,dup2_impl,dup2_x1_impl,dup2_x2_impl,swap_impl,iadd_impl,ladd_impl,fadd_impl,dadd_impl,isub_impl,lsub_impl,fsub_impl,dsub_impl,imul_impl,lmul_impl,fmul_impl,dmul_impl,idiv_impl,ldiv_impl,fdiv_impl,ddiv_impl,irem_impl,lrem_impl,frem_impl,drem_impl,ineg_impl,lneg_impl,fneg_impl,dneg_impl,ishl_impl,lshl_impl,ishr_impl,lshr_impl,iushr_impl,lushr_impl,iand_impl,land_impl,ior_impl,lor_impl,ixor_impl,lxor_impl,iinc_fantasma,i2l_impl,i2f_impl,i2d_impl,l2i_impl,l2f_impl,l2d_impl,f2i_impl,f2l_impl,f2d_impl,d2i_impl,d2l_impl,d2f_impl,i2b_impl,i2c_impl,i2s_impl,lcmp_impl,fcmpl_impl,fcmpg_impl,dcmpl_impl,dcmpg_impl,ifeq_impl,ifne_impl,iflt_impl,ifge_impl,ifgt_impl,ifle_impl,if_icmpeq_impl,if_icmpne_impl,if_icmplt_impl,if_icmpge_impl,if_icmpgt_impl,if_icmple_impl,acmpeq_impl,acmpne_impl,inst_goto_impl,jsr_impl,ret_impl,tableswitch_fantasma,lookupswitch_fantasma,ireturn_impl,lreturn_impl,freturn_impl,dreturn_impl,areturn_impl,inst_return_impl,getstatic_impl,putstatic_impl,getfield_impl,putfield_impl,invokevirtual_impl,invokespecial_impl,invokestatic_impl,invokeinterface_fantasma,invokedynamic_fantasma,inst_new_impl,newarray_impl,anewarray_impl,arraylength_impl,athrow_impl,checkcast_impl,instanceof_impl,monitorenter_impl,monitorexit_impl,wide_impl,multianewarray_fantasma,ifnull_impl,ifnonnull_impl,goto_w_impl,jsr_w_impl};

JVM* CriarJVM(){
	return NULL;
}

int menu (){
	int operacao = 0;
		printf("Selecione uma das opções da JVM:\n");
		printf("\t1 >> Leitor/Exibidor do arquivo .class\n");
		printf("\t2 >> Interpretador do arquivo .class\n");
		printf("\t0 >> Sair\n");
		scanf("%d",&operacao);
		getchar();
	return operacao;
}

JVM* InicializarJVM() {
	JVM *novo;
	novo = (JVM*)malloc(sizeof(JVM));
	novo->classes = CriarLista_classes();
	novo->frames = CriaPilhaFrames();
	novo->pc = 0;
	novo->excecao = 0;
	novo->objetos = NULL;
	novo->excecao_nome = malloc(100*sizeof(char));
	instrucoes = constroeInstrucoes();
	return novo;
}

frame* criarFrame(char *classeCorrente, u2 max_locals){
	frame *f = malloc(sizeof(frame));
	f->end_retorno = jvm->pc;
	f->p = CriaPilhaOperandos();
	f->v = malloc(max_locals*sizeof(vetor_locais));
	for (int i = 0; i < max_locals; i++) {
		f->v[i].variavel = malloc(sizeof(u4));
	}
	f->cp = BuscarCPClasseCorrente_classes(jvm->classes,classeCorrente);
	f->vetor_length = max_locals;
	f->classeCorrente = malloc(100*sizeof(char));
	strcpy(f->classeCorrente,classeCorrente);

	return(f);
}

void executarJVM(){

	method_info *metodos = jvm->classes->arquivoClass->methods;

	char * classeExecutavel = decodificaNIeNT(jvm->classes->arquivoClass->constant_pool,jvm->classes->arquivoClass->this_class,NAME_INDEX);

	for (method_info * methodAux = metodos; methodAux < metodos+jvm->classes->arquivoClass->methods_count; methodAux++) {
        char *stringmetodo = decodificaStringUTF8(jvm->classes->arquivoClass->constant_pool-1+methodAux->name_index);
        char *stringdescriptor = decodificaStringUTF8(jvm->classes->arquivoClass->constant_pool-1+methodAux->descriptor_index);
        if(strcmp(stringmetodo,"<clinit>")==0 && strcmp(stringdescriptor,"()V")==0 && methodAux->access_flags==0x0008){
            executarMetodo(methodAux,classeExecutavel,1);
        }

    }

	int i=0;

	for(method_info *aux=metodos;i<jvm->classes->arquivoClass->methods_count;aux++,i++){
		char *stringmetodo = decodificaStringUTF8(jvm->classes->arquivoClass->constant_pool-1+aux->name_index);
		char *stringdescriptor = decodificaStringUTF8(jvm->classes->arquivoClass->constant_pool-1+aux->descriptor_index);
		if(strcmp(stringmetodo, "main")==0 && strcmp(stringdescriptor,"([Ljava/lang/String;)V")==0 && aux->access_flags==0x0009){
			executarMetodo(aux,classeExecutavel,1);
		}
	}
}

void executarMetodo(method_info *m, char* classeCorrente, int chamador){
	attribute_info *aux;

	int posicao;

	for(posicao=0;posicao<m->attributes_count;posicao++){
		aux = (*(m->attributes+posicao));
		classesCarregadas *classeAtual = BuscarElemento_classes(jvm->classes,classeCorrente);
		char *nameindex = decodificaStringUTF8(classeAtual->arquivoClass->constant_pool-1+aux->attribute_name_index);
		if(strcmp(nameindex,"Code")==0){
			code_attribute *c = (code_attribute *) aux->info;
			if(chamador==1){
				frame *f = criarFrame(classeCorrente, c->max_locals);
				jvm->frames = PushFrames(jvm->frames,f);
			}

			interpretaCode(c->code,c->code_length,m);

		}
	}
}

void interpretaCode(u1 *code,u4 length,method_info *m){
	u1 opcode;
	int pcAtual;
	u2 handler_pc=0;
	for (u1 *j=code;j<code+length;){
		opcode = *j;
		pcAtual = jvm->pc;
		instrucao i = instrucoes[opcode];
		u1 numarg = i.numarg;
		j++;
		if(numarg>0){

			u1 *argumentos = malloc(numarg*sizeof(u1));
			for(u1 arg=0;arg<numarg;arg++){
				argumentos[arg] = *j;
				j++;
			}

			switch(numarg){
				case 1:
					(*func_ptr[i.opcode])(jvm->frames->topo->f,argumentos[0],0);
					if(jvm->excecao==1){
						handler_pc = verificaHandlerMetodo(m);
						if(handler_pc!=USHRT_MAX){
							jvm->excecao = 0;
							free(jvm->excecao_nome);
							jvm->excecao_nome = malloc(100*sizeof(char));
							jvm->pc = handler_pc;
						}
						jvm->pc = handler_pc;
						j = atualizarPCMetodoAtual(code,length);
						jvm->excecao = 0;
					}
					jvm->pc += i.pc_instrucao;
				break;

				case 2:
					(*func_ptr[i.opcode])(jvm->frames->topo->f,argumentos[0],argumentos[1]);
					if(jvm->excecao==1){
						handler_pc = verificaHandlerMetodo(m);
						if(handler_pc!=USHRT_MAX){
							jvm->excecao = 0;
							free(jvm->excecao_nome);
							jvm->excecao_nome = malloc(100*sizeof(char));
							jvm->pc = handler_pc;
						}
						jvm->pc = handler_pc;
						j = atualizarPCMetodoAtual(code,length);
						jvm->excecao = 0;
					}
					if (instrucaoBranch(i.inst_nome)) {
						if (pcAtual != jvm->pc) {
							j = atualizarPCMetodoAtual(code,length);
						} else {
							if(strcmp(i.inst_nome, "invokestatic") != 0 ||
							strcmp(i.inst_nome, "invokevirtual") != 0 ||
							strcmp(i.inst_nome, "invokespecial") != 0)
								jvm->pc += i.pc_instrucao;

						}
					} else {
						jvm->pc += i.pc_instrucao;
					}
				break;

				case 3:
					if(strcmp(i.inst_nome, "multianewarray")==0){
						multianewarray_impl(jvm->frames->topo->f, argumentos[0],argumentos[1],argumentos[2]);
						if(jvm->excecao==1){
							handler_pc = verificaHandlerMetodo(m);
							if(handler_pc!=USHRT_MAX){
								jvm->excecao = 0;
								free(jvm->excecao_nome);
								jvm->excecao_nome = malloc(100*sizeof(char));
								jvm->pc = handler_pc;
							}
							jvm->pc = handler_pc;
							j = atualizarPCMetodoAtual(code,length);
						}
					}
				break;

				case 4:
					if(strcmp(i.inst_nome,"invokeinterface")==0){
						invokeinterface_impl(jvm->frames->topo->f,argumentos[0],argumentos[1],argumentos[2]);
						if(jvm->excecao==1){
							handler_pc = verificaHandlerMetodo(m);
							if(handler_pc!=USHRT_MAX){
								jvm->excecao = 0;
								free(jvm->excecao_nome);
								jvm->excecao_nome = malloc(100*sizeof(char));
								jvm->pc = handler_pc;
							}
							jvm->pc = handler_pc;
							j = atualizarPCMetodoAtual(code,length);
							jvm->excecao = 0;
						}
					}
				break;
			}
		}
		else if(numarg==0){
			if(strcmp(i.inst_nome,"wide")==0){
				u1 novo_opcode = *j;
				i = instrucoes[novo_opcode];
				j++;
				if(novo_opcode!=iinc){
					u1 *argumentos = malloc(numarg*sizeof(u1));
					for(u1 arg=0;arg<i.numarg+1;arg++){
						argumentos[arg] = *j;
						j++;
					}

					(*func_ptr[i.opcode])(jvm->frames->topo->f,argumentos[0],argumentos[1]);

					jvm->pc += i.numarg+1;
				}
				else{
					u1 *argumentos = malloc(i.numarg*2*sizeof(u1));

					for (u1 arg=0;arg<i.numarg*2;arg++){
						argumentos[arg] = *j;
						j++;
					}

					iinc_wide_fantasma(jvm->frames->topo->f,argumentos[0],argumentos[1],argumentos[2],argumentos[3]);

					jvm->pc += i.numarg*2-1;
				}
			}
			else{

				(*func_ptr[i.opcode])(jvm->frames->topo->f,0,0);
			}

			if(jvm->excecao==1){
				handler_pc = verificaHandlerMetodo(m);
				if(handler_pc!=USHRT_MAX){
					free(jvm->excecao_nome);
					jvm->excecao_nome = malloc(100*sizeof(char));
					jvm->pc = handler_pc;
				}
				jvm->pc = handler_pc;
				j = atualizarPCMetodoAtual(code,length);
				jvm->excecao = 0;
			}
			jvm->pc += i.pc_instrucao;
		}
	}
}

void freeVetorLocais(vetor_locais *v, u2 vetorLength){
	int cont = 0;
	while(cont<vetorLength){
		free(v[cont].variavel);
		cont++;
	}
	free(v);
}

u2 verificaHandlerMetodo(method_info *m){

	attribute_info *aux;
	classesCarregadas *classeAtual = BuscarElemento_classes(jvm->classes,jvm->frames->topo->f->classeCorrente);
	method_info *auxmetodo;

	while(jvm->frames->topo!=NULL){

		for(auxmetodo=m;auxmetodo<m+m->attributes_count;auxmetodo++){
			aux = (*auxmetodo->attributes);
			char *nameindex = decodificaStringUTF8(classeAtual->arquivoClass->constant_pool-1+aux->attribute_name_index);
			if(strcmp(nameindex,"Code")==0){
				code_attribute *c = (code_attribute *) aux->info;
				for(exception_table *tabelaaux = c->table;tabelaaux<c->table+c->exception_table_length;tabelaaux++){
					char *nomeexcecao = decodificaNIeNT(classeAtual->arquivoClass->constant_pool,tabelaaux->catch_type,NAME_INDEX);
					if(strcmp(nomeexcecao,jvm->excecao_nome)==0){
						return(tabelaaux->handler_pc);
					}
				}
			}
		}

		while(jvm->frames->topo->f->p->topo!=NULL){
			PopOperandos(jvm->frames->topo->f->p);
		}

		freeVetorLocais(jvm->frames->topo->f->v,jvm->frames->topo->f->vetor_length);

		PopFrames(jvm->frames);
	}

	return(USHRT_MAX);
}

bool instrucaoBranch (char * nomeInstrucao) {
	bool isBranch = false;
	if (strcmp(nomeInstrucao,"goto") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"if_icmpgt") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"ifne") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"ifeq") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"iflt") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"ifge") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"ifle") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"ifgt") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"if_icmpeq") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"if_icmpne") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"if_icmplt") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"if_icmpge") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"if_icmple") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"if_acmpeq") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"if_acmpne") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"jsr") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"ifnonnull") == 0) {
		isBranch = true;
	} else if (strcmp(nomeInstrucao,"ifnull") == 0) {
		isBranch = true;
	} else {
		isBranch = false;
	}
	return isBranch;
}

u1 * atualizarPCMetodoAtual (u1 * code, u4 length) {
	int contador = 0;
	int numArgs = 0;
	u1 opcode;
	for (u1 * aux = code; aux < code+length; aux++) {
			opcode = *aux;
			instrucao i = instrucoes[opcode];
			contador += i.pc_instrucao;
			numArgs = i.numarg;
			if(numArgs>0){
				for(u1 arg = 0; arg < numArgs; arg++) {
					aux++;
				}
			}
			if (contador == jvm->pc) {
				aux++;
				return aux;
			}
	}
	return 0;
}

void ImprimeCode (u1 * code, u4 length) {
	int numArgs = 0;
	for (u1 * saida = code; saida < code+length; saida++) {
		u1 opcodeaux = *saida;
		instrucao iaux = instrucoes[opcodeaux];
		numArgs = iaux.numarg;
		if(numArgs>0){
			for(u1 arg = 0; arg < numArgs; arg++) {
				saida++;
			}
		}
	}
}
