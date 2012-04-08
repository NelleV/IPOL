%-------------------------------------------------------------------------------
% ipol.cls
% LaTeX class for IPOL articles
%
% rafael grompone von gioi - 2012.1.27
%-------------------------------------------------------------------------------
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{ipol}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\ProcessOptions
\LoadClass[a4paper,12pt]{article}

% Margins
\RequirePackage[cm]{fullpage}

% Links package and colors definition
\RequirePackage{color}
\definecolor{link}{rgb}{0.357,0.286,0.651}
\RequirePackage[colorlinks,
                linkcolor=link,
                anchorcolor=link,
                citecolor=link,
                filecolor=link,
                menucolor=link,
                pagecolor=link,
                urlcolor=link]{hyperref}

% IPOL environments
\renewenvironment{abstract}{\small%
                            \begin{center}%
                            {\bf Abstract\vspace{-.5em}}%
                            \end{center}%
                            \begin{quote}%
                            \noindent\ignorespaces}{\end{quote}}
\newenvironment{ipolCode}{\small%
                          \begin{center}%
                          {\bf Source Code\vspace{-.5em}}%
                          \end{center}%
                          \begin{quote}%
                          \noindent\ignorespaces}{\end{quote}}
\newenvironment{ipolSupp}{\small%
                          \begin{center}%
                          {\bf Supplementary Material\vspace{-.5em}}%
                          \end{center}%
                          \begin{quote}%
                          \noindent\ignorespaces}{\end{quote}}

% IPOL data
\newcommand{\ipolTitle}{title}
\newcommand{\ipolAuthors}{authors}
\newcommand{\ipolYear}{YYYY}
\newcommand{\ipolMonth}{MM}
\newcommand{\ipolDay}{DD}
\newcommand{\ipolID}{XXXXXXXX}
\newcommand{\ipolDOI}{10.5201/ipol.\ipolYear.\ipolID}
\newcommand{\ipolLink}{http://dx.doi.org/\ipolDOI}

% IPOL data set commands
\newcommand{\ipolSetTitle}[1]{\renewcommand{\ipolTitle}{#1}%
                              \newcommand{\ipolTitleDefined}{}}
\newcommand{\ipolSetAuthors}[1]{\renewcommand{\ipolAuthors}{#1}%
                                \newcommand{\ipolAuthorsDefined}{}}
\newcommand{\ipolSetYear}[1]{\renewcommand{\ipolYear}{#1}%
                             \newcommand{\ipolYearDefined}{}}
\newcommand{\ipolSetMonth}[1]{\renewcommand{\ipolMonth}{#1}%
                              \newcommand{\ipolMonthDefined}{}}
\newcommand{\ipolSetDay}[1]{\renewcommand{\ipolDay}{#1}%
                            \newcommand{\ipolDayDefined}{}}
\newcommand{\ipolSetID}[1]{\renewcommand{\ipolID}{#1}%
                           \newcommand{\ipolIDDefined}{}}
\newcommand{\ipolDraftLink}[1]{\renewcommand{\ipolLink}{#1}%
                               \newcommand{\ipolDraft}{}}

% Print IPOL Header and Title
\RequirePackage{graphicx}
\AtBeginDocument{%
\includegraphics[width=.27\linewidth]{ipol}%
\hspace{1em}%
\vbox{\scriptsize\sf\noindent%
Published in \href{http://dx.doi.org/10.5201/ipol}{Image Processing On Line}
on \ipolYear--\ipolMonth--\ipolDay.\\
ISSN 2105--1232 \copyright\ \ipolYear\ IPOL
\href{http://creativecommons.org/licenses/by-nc-sa/3.0/}{CC--BY--NC--SA}\\
This article is available online with supplement materials,\\
implementation, datasets and demonstration at\\
\url{\ipolLink}}
\begin{center}
  \Large\sf\color{red}\phantom{DRAFT}%
  \@ifundefined{ipolTitleDefined}{DRAFT}{%
    \@ifundefined{ipolAuthorsDefined}{DRAFT}{%
      \@ifundefined{ipolYearDefined}{DRAFT}{%
        \@ifundefined{ipolMonthDefined}{DRAFT}{%
          \@ifundefined{ipolDayDefined}{DRAFT}{%
            \@ifundefined{ipolIDDefined}{DRAFT}{%
              \@ifundefined{ipolDraft}{}{DRAFT}}}}}}}%
  \phantom{DRAFT}%
\end{center}
\begin{center}
\Large\textbf{\ipolTitle}\\
\bigskip
\large\ipolAuthors\\
\bigskip
\end{center}
}

%-------------------------------------------------------------------------------
