#' Genocut
#'
#' @param file FASTA file with multiple sequences
#' @param len length of the sequence to be cut
#' @param num Number of sequence to be generated 
#'
#' @return DNAStringSet object
#' 
#' @examples
#' dat <- genocut ("test.fas", 300, 10)
#' 
#' @useDynLib genocut
#' @exportPattern "^[[:alpha:]]+"
#' @export
genocut <- function (file, len=0, num=0)
{
    if (len==0) stop ("Parameter length (len) must be set.")
    if (num==0) stop ("Paramter number of sequence (num) must be set.")

    fstr <- .Call ("getseqs", file, len, num)

    lsq <- str_split_1 (fstr,"\n")
    dss <- DNAStringSet (lsq[seq (2,length(lsq),2)])
    names(dss) <- lsq[seq (1,length(lsq),2)]

    return (dss)
}