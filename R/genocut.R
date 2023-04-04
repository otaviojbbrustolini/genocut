#' Genocut
#'
#' @param file FASTA file with multiple sequences
#' @param len length of the sequence to be cut
#' @param num Number of sequence to be generated 
#'
#' @return character vector
#' 
#' @examples
#' dat <- genocut ("test.fas", 300, 10)
#' 
#' @export
genocut <- function (file, len=0, num=0)
{
    if (len==0) stop ("Parameter length (len) must be set.")
    if (num==0) stop ("Paramter number of sequence (num) must be set.")

    str <- .Call ("getseqs", len, num)

    return (str)
}