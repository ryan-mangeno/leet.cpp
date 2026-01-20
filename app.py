import streamlit as st
import subprocess
import os
from streamlit_ace import st_ace

st.set_page_config(layout="wide", page_title="Local C++ Dojo", page_icon="⚡")

PROBLEMS_DIR = "problems"

st.markdown("""
<style>
    .stApp { background-color: #0d1117; color: #c9d1d9; }
    .stButton button { background-color: #238636; color: white; border: none; font-weight: bold; }
    .stSelectbox div[data-baseweb="select"] > div { background-color: #161b22; color: #c9d1d9; }
    div[data-testid="stSidebar"] { background-color: #010409; border-right: 1px solid #30363d; }
    code { color: #ff7b72; }
</style>
""", unsafe_allow_html=True)

def get_problems():
    """Scans the problems directory for subfolders"""
    if not os.path.exists(PROBLEMS_DIR):
        os.makedirs(PROBLEMS_DIR)
    return sorted([f for f in os.listdir(PROBLEMS_DIR) if os.path.isdir(os.path.join(PROBLEMS_DIR, f))])

def load_file(problem_dir, filename):
    path = os.path.join(problem_dir, filename)
    if os.path.exists(path):
        with open(path, "r") as f:
            return f.read()
    return f"// Error: {filename} not found."

st.sidebar.title("Problem Set")
problem_list = get_problems()

if not problem_list:
    st.error(f"No problems found in '{PROBLEMS_DIR}/'. Run the setup script!")
    st.stop()

selected_problem = st.sidebar.radio("Select a Challenge:", problem_list)

# We define the specific folder for the selected problem
current_problem_dir = os.path.join(PROBLEMS_DIR, selected_problem)

# We load assets from THAT folder
prompt_text = load_file(current_problem_dir, "prompt.md")

prior_sub_path = os.path.join(current_problem_dir, "submission.h")
prior_sub = None
if os.path.exists(prior_sub_path):
    prior_sub = load_file(current_problem_dir, "submission.h")

starter_code = None
if not prior_sub:
    starter_code = load_file(current_problem_dir, "starter.h")

# We define where the submission and runner go (inside that folder)
submission_path = os.path.join(current_problem_dir, "submission.h")
test_harness_path = os.path.join(current_problem_dir, "test_harness.cpp")
runner_exe = os.path.join(current_problem_dir, "runner")

# SESSION STATE
session_key = f"code_{selected_problem}"
if session_key not in st.session_state:
    if prior_sub:
        st.session_state[session_key] = prior_sub
    else:
        st.session_state[session_key] = starter_code

# MAIN UI
col1, col2 = st.columns([1, 2])

with col1:
    st.markdown(f"## {selected_problem.replace('_', ' ').title()}")
    st.markdown(prompt_text)
    
    st.markdown("---")
    st.markdown("### Console")
    output_key = f"output_{selected_problem}"
    if output_key in st.session_state:
        st.code(st.session_state[output_key], language="text")

with col2:
    st.markdown(f"## Code Editor ({selected_problem}/submission.h)")
    
    code_input = st_ace(
        value=st.session_state[session_key],
        language="c_cpp",
        theme="monokai",
        keybinding="vscode",
        font_size=14,
        tab_size=4,
        height=600,
        show_gutter=True,
        wrap=True,
        auto_update=False,
        key=f"ace_{session_key}"
    )
    
    st.session_state[session_key] = code_input

    if st.button("Run Tests", use_container_width=True):
        # should be based on problem but quick fix for now 
        forbidden = ["std::mutex", "std::lock_guard", "memory_order_seq_cst"]
        
        if "spsc" in selected_problem:
            violations = [term for term in forbidden if term in code_input]
        else:
            violations = []
        
        if violations:
             st.session_state[output_key] = f"FORBIDDEN: {', '.join(violations)}\nLock-free only!"
        else:
            with open(submission_path, "w") as f:
                f.write(code_input)
            
            compile_cmd = [
                "g++", 
                "-std=c++17", 
                "-pthread", 
                f"-I{current_problem_dir}", 
                test_harness_path, 
                "-o", runner_exe
            ]
            
            build = subprocess.run(compile_cmd, capture_output=True, text=True)
            
            if build.returncode != 0:
                st.session_state[output_key] = f"🔥 BUILD ERROR:\n{build.stderr}"
            else:
                try:
                    run = subprocess.run([runner_exe], capture_output=True, text=True, timeout=2)
                    st.session_state[output_key] = run.stdout + "\n" + run.stderr
                except subprocess.TimeoutExpired:
                     st.session_state[output_key] = "⏱TIMEOUT: Possible infinite loop/deadlock."
                
                if os.path.exists(runner_exe): os.remove(runner_exe)
                
        st.rerun()