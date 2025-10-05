import pandas as pd
import matplotlib.pyplot as plt

try:
    # Read data from both of your CSV files
    df_bst = pd.read_csv("bst_results.csv")
    df_ost = pd.read_csv("ost_results.csv")
except FileNotFoundError as e:
    print(f"Error: Make sure the file '{e.filename}' is in the same directory as the script.")
    exit()


# --- Figure 1: BST Performance (Part A) ---
fig, axs = plt.subplots(2, 1, figsize=(10, 12))
fig.suptitle('Binary Search Tree Performance (Part A)', fontsize=16)

# Top subplot: Expected Height
axs[0].plot(df_bst["n"], df_bst["ExpectedHeight"], marker='o')
axs[0].set_title("Expected Height vs. n")
axs[0].set_xlabel("n (Number of Keys)")
axs[0].set_ylabel("Expected Height")
axs[0].grid(True)

# Bottom subplot: Operation Runtimes
axs[1].plot(df_bst["n"], df_bst["AvgBuildTime_ms"], marker='o', label="Build Time (ms)")
axs[1].plot(df_bst["n"], df_bst["AvgDestroyTime_ms"], marker='s', label="Destroy Time (ms)")
axs[1].plot(df_bst["n"], df_bst["AvgWalkTime_ms"], marker='^', label="Walk Time (ms)")
axs[1].set_title("Average Operation Runtimes vs. n")
axs[1].set_xlabel("n (Number of Keys)")
axs[1].set_ylabel("Average Time (ms)")
axs[1].legend()
axs[1].grid(True)

plt.tight_layout(rect=[0, 0, 1, 0.96]) 
plt.savefig("bst_performance.png", dpi=300)
plt.show()


# --- Figure 2: OST Runtimes (Comparison) ---
plt.figure(figsize=(10, 6))

# Plot BST times with dashed lines
plt.plot(df_bst["n"], df_bst["AvgBuildTime_ms"], marker='o', linestyle='--', label="BST Build Time (ms)")
plt.plot(df_bst["n"], df_bst["AvgDestroyTime_ms"], marker='s', linestyle='--', label="BST Destroy Time (ms)")

# Plot OST times with solid lines
plt.plot(df_ost["n"], df_ost["AvgBuildTime_ms"], marker='o', label="OST Build Time (ms)")
plt.plot(df_ost["n"], df_ost["AvgDestroyTime_ms"], marker='s', label="OST Destroy Time (ms)")

plt.title("Comparison of BST vs. OST Runtimes")
plt.xlabel("n (Number of Keys)")
plt.ylabel("Average Time (ms)")
plt.legend()
plt.grid(True)

plt.savefig("comparison.png", dpi=300)
plt.show()


# --- Figure 3: OST Query Runtimes (Part B) ---
plt.figure(figsize=(10, 6))

plt.plot(df_ost["n"], df_ost["AvgSelectTime_ms"], marker='^', label="OS-SELECT Time (ms)")
plt.plot(df_ost["n"], df_ost["AvgRankTime_ms"], marker='x', label="OS-RANK Time (ms)")

plt.title("Runtimes for Order-Statistic Query Operations")
plt.xlabel("n (Number of Keys)")
plt.ylabel("Average Time (ms)")
plt.legend()
plt.grid(True)

plt.savefig("ost_query_times.png", dpi=300)
plt.show()