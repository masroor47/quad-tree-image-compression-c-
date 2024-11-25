import os
import re

class DocumentGenerator:
    def __init__(self):
        self.latex_template = r"""
\documentclass[12pt]{article}
\usepackage{listings}
\usepackage{xcolor}
\usepackage{geometry}
\usepackage{hyperref}
\usepackage{bookmark}

\geometry{
    a4paper,
    left=1in,
    right=1in,
    top=1in,
    bottom=1in
}

\lstset{
    frame=single,
    breaklines=true,
    postbreak=\raisebox{0ex}[0ex][0ex]{\ensuremath{\hookrightarrow\space}},
    numbers=left,
    numberstyle=\tiny\color{gray},
    basicstyle=\ttfamily\small,
    keywordstyle=\color{blue},
    commentstyle=\color{green!60!black},
    stringstyle=\color{orange},
    showstringspaces=false
}

\begin{document}

\title{CSCI 323 Design and Analysis of Algorithms}
\author{Masroor Khonkhodzhaev}
\maketitle

\tableofcontents
\newpage

%s

\end{document}
"""

    def read_file_content(self, filepath, max_lines=None):
        """Read file content with optional line limit"""
        try:
            with open(filepath, 'r') as file:
                if max_lines:
                    lines = []
                    for i, line in enumerate(file):
                        if i >= max_lines:
                            lines.append('... (output truncated after {} lines)'.format(max_lines))
                            break
                        lines.append(line)
                    return ''.join(lines)
                return file.read()
        except FileNotFoundError:
            return f"Error: File {filepath} not found"
        except Exception as e:
            return f"Error reading file {filepath}: {str(e)}"

    def escape_latex(self, text):
        """Escape special LaTeX characters"""
        chars = {
            '&': r'\&',
            '%': r'\%',
            '$': r'\$',
            '#': r'\#',
            '_': r'\_',
            '{': r'\{',
            '}': r'\}',
            '~': r'\textasciitilde{}',
            '^': r'\^{}',
            '\\': r'\textbackslash{}',
        }
        pattern = '|'.join(re.escape(key) for key in chars.keys())
        return re.sub(pattern, lambda m: chars[m.group()], text)

    def generate_document(self, source_files, image_files):
        """Generate LaTeX document from source and output files"""
        content = []
        
        # Add source code sections
        content.append(r'\section{Source Code}')
        
        # First add header files
        content.append(r'\subsection{Header Files}')
        for header_file in [f for f in source_files if f.endswith('.h')]:
            filename = os.path.basename(header_file)
            content.append(f'\subsubsection{{{filename}}}')
            content.append(r'\begin{lstlisting}[language=C++]')
            content.append(self.read_file_content(header_file))
            content.append(r'\end{lstlisting}')
            content.append(r'\newpage')

        # Then add implementation files
        content.append(r'\subsection{Implementation Files}')
        for impl_file in [f for f in source_files if f.endswith('.cpp')]:
            filename = os.path.basename(impl_file)
            content.append(f'\subsubsection{{{filename}}}')
            content.append(r'\begin{lstlisting}[language=C++]')
            content.append(self.read_file_content(impl_file))
            content.append(r'\end{lstlisting}')
            content.append(r'\newpage')

        # Add sections for each image output and log
        content.append(r'\section{Program Output}')
        for i, (out_file, log_file) in enumerate(image_files, 1):
            content.append(f'\subsection{{Image {i} Results}}')
            
            # Output file
            content.append(r'\subsubsection{Program Output}')
            content.append(r'\begin{lstlisting}')
            content.append(self.escape_latex(self.read_file_content(out_file)))
            content.append(r'\end{lstlisting}')
            
            # Log file (limited to 3 pages worth of content - approximately 150 lines)
            content.append(r'\subsubsection{Log File}')
            content.append(r'\begin{lstlisting}')
            content.append(self.escape_latex(self.read_file_content(log_file, max_lines=150)))
            content.append(r'\end{lstlisting}')
            content.append(r'\newpage')

        # Combine all content
        full_content = '\n'.join(content)
        return self.latex_template % full_content

    def save_document(self, output_path, content):
        """Save the LaTeX document"""
        try:
            with open(output_path, 'w') as file:
                file.write(content)
            return True
        except Exception as e:
            print(f"Error saving document: {str(e)}")
            return False

def main():
    # Initialize generator
    generator = DocumentGenerator()
    
    # Define your source files here
    source_files = [
        '../headers/QtNode.h',
        '../headers/QuadTree.h',
        '../src/QtNode.cpp',
        '../src/QuadTree.cpp',
        '../src/main.cpp'
    ]
    
    # Define your output and log files here
    image_files = [
        ('../outputs/output1.txt', '../outputs/log1.txt'),
        ('../outputs/output2.txt', '../outputs/log2.txt'),
        ('../outputs/output3.txt', '../outputs/log3.txt')
    ]
    
    # Generate and save the document
    content = generator.generate_document(source_files, image_files)
    if generator.save_document('assignment.tex', content):
        print("Document generated successfully as 'assignment.tex'")
        print("Run 'pdflatex assignment.tex' to generate the PDF")
    else:
        print("Failed to generate document")

if __name__ == "__main__":
    main()