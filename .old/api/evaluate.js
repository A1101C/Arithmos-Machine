// src/pages/api/evaluate.js
import { execFile } from 'child_process';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

export async function POST({ request }) {
    try {
        const body = await request.json();

        const { mode, expression, xMin, xMax, xCount } = body;

        if (!expression) {
            return new Response(JSON.stringify({ error: "No expression provided" }), {
                status: 400,
                headers: { 'Content-Type': 'application/json' }
            });
        }

        const binaryPath = path.join(__dirname, '../../../../output/engine');
        
        const args = [mode || "s", expression];

        if (mode === "g") {
            args.push(xMin || "0", xMax || "0", xCount || "0");
        }

        const result = await new Promise((resolve, reject) => {
            execFile(binaryPath, args, (error, stdout, stderr) => {
                if (error) {
                    reject({ status: 500, error: "Engine Execution Failed", details: stderr || error.message });
                    return;
                }

                const points = [];

                if (mode === "g" && stdout) {
                    const lines = stdout.split('\n');
                    for (const line of lines) {
                        const match = line.match(/\(([^,]+),\s*([^)]+)\)/);
                        if (match) {
                            points.push({ x: match[1].trim(), y: match[2].trim() });
                        }
                    }
                }

                resolve({
                    success: true,
                    input: expression,
                    result: stdout,
                    points: points.length > 0 ? points : null
                });
            });
        });

        return new Response(JSON.stringify(result), {
            status: 200,
            headers: { 'Content-Type': 'application/json' }
        });

    } catch (err) {
        const status = err.status || 500;
        return new Response(JSON.stringify({ error: err.error || "Internal Server Error", details: err.details || err.message }), {
            status: status,
            headers: { 'Content-Type': 'application/json' }
        });
    }
}