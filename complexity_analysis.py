import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


df_bst = pd.read_csv("bst_results.csv")
df_ost = pd.read_csv("ost_results.csv")


# --- Plot 1: BST Height (Log-Linear Scale) ---
plt.figure(figsize=(8, 6))
plt.semilogx(df_bst["n"], df_bst["ExpectedHeight"], marker='o')
plt.title("BST Height vs. n (Log-Linear Scale)")
plt.xlabel("n (Log Scale)")
plt.ylabel("Expected Height (Linear Scale)")
plt.grid(True)
plt.savefig("complexity_height.png", dpi=300)
plt.show()

# --- Plot 2: Walk Time (Linear Scale) ---
plt.figure(figsize=(8, 6))
plt.plot(df_bst["n"], df_bst["AvgWalkTime_ms"], marker='^')
plt.title("Inorder Walk Time vs. n (Linear Scale)")
plt.xlabel("n (Linear Scale)")
plt.ylabel("Average Time (ms)")
plt.grid(True)
plt.savefig("complexity_walk.png", dpi=300)
plt.show()

# --- Plot 3: Query Times (Log-Linear Scale) ---
plt.figure(figsize=(8, 6))
plt.semilogx(df_ost["n"], df_ost["AvgSelectTime_ms"], marker='^', label="OS-SELECT Time")
plt.semilogx(df_ost["n"], df_ost["AvgRankTime_ms"], marker='x', label="OS-RANK Time")
plt.title("Query Times vs. n (Log-Linear Scale)")
plt.xlabel("n (Log Scale)")
plt.ylabel("Average Time (ms)")
plt.legend()
plt.grid(True)
plt.savefig("complexity_query.png", dpi=300)
plt.show()

# --- Plot 4: Build/Destroy Times (Log-Log Scale) ---
plt.figure(figsize=(8, 6))
plt.loglog(df_bst["n"], df_bst["AvgBuildTime_ms"], marker='o', linestyle='--', label="BST Build")
plt.loglog(df_ost["n"], df_ost["AvgBuildTime_ms"], marker='o', label="OST Build")
plt.loglog(df_bst["n"], df_bst["AvgDestroyTime_ms"], marker='s', linestyle='--', label="BST Destroy")
plt.loglog(df_ost["n"], df_ost["AvgDestroyTime_ms"], marker='s', label="OST Destroy")
plt.title("Build/Destroy Times vs. n (Log-Log Scale)")
plt.xlabel("n (Log Scale)")
plt.ylabel("Average Time (ms) (Log Scale)")
plt.legend()
plt.grid(True, which="both", ls="--")
plt.savefig("complexity_build_destroy.png", dpi=300)
plt.show()