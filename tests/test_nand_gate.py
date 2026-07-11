import cocotb
from cocotb.triggers import Timer


@cocotb.test()
async def nand_truth_table_test(dut):
    test_cases = [(0, 0, 1), (0, 1, 1), (1, 0, 1), (1, 1, 0)]

    for a_val, b_val, expected_y in test_cases:
        dut.a.value = a_val
        dut.b.value = b_val

        await Timer(1, unit="ns")

        actual_y = dut.y.value
        assert actual_y == expected_y, (
            f"Failed! A={a_val} B={b_val} | Expected {expected_y}, Got {actual_y}"
        )

        cocotb.log.info(f"Input: A={a_val} B={b_val} -> Y={actual_y}")
