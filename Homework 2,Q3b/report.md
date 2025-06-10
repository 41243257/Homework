# 41243257、41243234

作業二的第三題(b)

## 解題說明

本題要求寫一個 畫出一個總輸入tinput函數圖形。是否有一個值使得tinput就近似tcpu。

### 解題策略

(2)按照公式進行運算

(3)主程式呼叫公式進行運算輸出

---

### 計畫實作

以下為主要課程代碼：

```python


import numpy as np
import matplotlib.pyplot as plt

# ── 題目參數 

ts = 80e-3          # seek time  (80 ms)   — 此範例未細算，保留變數
t1 = 20e-3          # 旋轉延遲 (20 ms)      — 此範例未細算，保留變數
n  = 200_000        # 總紀錄數
m  = 64             # 可用緩衝頁面數 ⇒ k ≤ m-1
tr = 1e-3           # 傳輸時間 (1 ms / record)
S  = 2_000          # 初始 run 數

# ── k 範圍：2到63（m-1）

k_vals = np.arange(2, m)       # 2 到 64（含 63）

# ── 需要的 merge pass：ceil(log_k S) 

passes = np.ceil(np.log(S) / np.log(k_vals))

# ── 每個 pass 的 I/O 時間（讀 + 寫 n 筆）────────────────

t_per_pass = 2 * n * tr        # = 400 s；

# ── 總 t_input(k) 

t_input = passes * t_per_pass   # 單位：秒

# ── 假設 CPU 合併時間為常數

t_CPU = 800

# ── 繪圖 

plt.figure(figsize=(10, 5))

# 階梯圖：I/O 時間

plt.step(k_vals, t_input, where='post', label='t_input(k)')

# 水平虛線：CPU 時間

plt.axhline(
y=t_CPU,
color='red',
alpha=0.7,
linestyle='--',
linewidth=2,
label='t_CPU = 800 s'
)

# 標註 k ≥ 45 時重疊（只需 2 pass）

plt.annotate(
't_input = t_CPU (k ≥ 45)',
xy=(45, t_CPU),
xytext=(30, t_CPU + 150),
arrowprops=dict(arrowstyle='->')
)

plt.xlabel('k (k-way merge fan-in)')
plt.ylabel('Total Input Time t_input (seconds)')
plt.title('t_input vs. k  (n = 200,000, S = 2000)')
plt.xticks(k_vals[::5])  # 每隔 5 個 k 標一次刻度
plt.grid(True, linestyle=':')
plt.legend()
plt.tight_layout()
plt.show()


```

## 狀況分析

1. **時間複雜度：** O(n),原本是O(n logₖ S)（但k、S 為常數可簡寫)
2. **空間複雜度：** O(k)


## 測試與驗證

### 測試案例
![output (1)](https://github.com/user-attachments/assets/72c58855-9193-473e-b927-d23f44eae5af)


### 編譯與執行指令

````shellpip
pip install numpy matplotlib

python3 plot_k_merge.py

````

### 結論

1.**k ≥ 45** 時，tipunt就近似tcpu。

## 申論及開發報告

由於c++對於繪圖來說太難了，改用python來解釋，也就是他要求的題目，需要透過大量程式與公式，原本設置tcpu是60但完全沒接近近似，於是在不斷的修改中找到了剛好的k值，也就是45，並且按照題目要求符合近似值。
而且python還能夠在圖形上增加註釋。

```python
plt.annotate(
't_input = t_CPU (k ≥ 45)',
xy=(45, t_CPU),
xytext=(30, t_CPU + 150),
arrowprops=dict(arrowstyle='->')
)
```

