        !COMPILER-GENERATED INTERFACE MODULE: Mon Jun 27 17:35:46 2016
        MODULE RDHYD__genmod
          INTERFACE 
            SUBROUTINE RDHYD(NFILES,LUNIT,FNAM,NOLAY,IHDEL,TCKTOT,NDOMS,&
     &NBNDS,DOMS,BNDS)
              USE TYPOS
              INTEGER(KIND=4), INTENT(IN) :: NFILES
              INTEGER(KIND=4), INTENT(INOUT) :: LUNIT(NFILES)
              CHARACTER(LEN=256), INTENT(INOUT) :: FNAM(NFILES)
              INTEGER(KIND=4), INTENT(OUT) :: NOLAY
              INTEGER(KIND=4), INTENT(OUT) :: IHDEL
              REAL(KIND=4) ,POINTER :: TCKTOT(:)
              INTEGER(KIND=4), INTENT(OUT) :: NDOMS
              INTEGER(KIND=4), INTENT(OUT) :: NBNDS
              TYPE (DOMAIN) ,POINTER :: DOMS(:)
              TYPE (BOUNDP) ,POINTER :: BNDS(:)
            END SUBROUTINE RDHYD
          END INTERFACE 
        END MODULE RDHYD__genmod